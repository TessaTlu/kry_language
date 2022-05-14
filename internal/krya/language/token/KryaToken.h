#ifndef KRYA_TOKEN_H
#define KRYA_TOKEN_H
#include <unordered_map>
#include <vector>
#include <string_view>
#include <regex>
#include <boost/regex.hpp>
struct KryaToken {
    enum class Kryakva {
        OPERATOR,
        ASSIGN_OPERATOR,
        OPEN_PARENTHESIS,
        CLOSE_PARENTHESIS,
        NUMBER_CONSTANT,
        STRING_CONSTANT,
        IDENTIFIER,
        ENDLINE,
        SPACE,
        TAB,
        COMMA,
        SEMICOLON,
        UNDEFINED
    };
    const Kryakva type;
    constexpr const char* KryaToTheWorld(Kryakva e) throw()
    {
        switch (e)
        {
            case Kryakva::OPERATOR: return "KRYA_OPERATOR";
            case Kryakva::ASSIGN_OPERATOR: return "KRYA_ASSIGN_OPERATOR";
            case Kryakva::OPEN_PARENTHESIS: return "KRYA_OPEN_PARENTHESIS";
            case Kryakva::CLOSE_PARENTHESIS: return "KRYA_CLOSE_PARENTHESIS";
            case Kryakva::NUMBER_CONSTANT: return "KRYA_NUMBER_CONSTANT";
            case Kryakva::STRING_CONSTANT: return "KRYA_STRING_CONSTANT";
            case Kryakva::IDENTIFIER: return "KRYA_IDENTIFIER";
            case Kryakva::ENDLINE: return "KRYA_ENDLINE";
            case Kryakva::SPACE: return "KRYA_SPACE";
            case Kryakva::TAB: return "KRYA_TAB";
            case Kryakva::COMMA: return "KRYA_COMMA";
            case Kryakva::SEMICOLON: return "KRYA_SEMICOLON";
            case Kryakva::UNDEFINED: return "KRYA_UNDEFINED";
            default: throw std::invalid_argument("Unimplemented item");
        }
    }
    explicit KryaToken(Kryakva type, std::string_view value = "")
            : type(type), value(std::string(value)){};
    const std::string value;
    const std::string type_name;
};
static const boost::regex IDENTIFIER_REGEX      ( R"([_]*[a-zA-Z][a-zA-Z0-9_]*)");
static const boost::regex STRING_CONSTANT_REGEX ( R"("(\\.|[^\\"])*")");
static const boost::regex NUMBER_CONSTANT_REGEX ( R"([+-]?\d+)");
static const std::map<std::string_view, KryaToken::Kryakva> OPERATORS = {
        {"+", KryaToken::Kryakva::OPERATOR},
        {"-", KryaToken::Kryakva::OPERATOR},
        {"*", KryaToken::Kryakva::OPERATOR},
        {"/", KryaToken::Kryakva::OPERATOR},
        {"%", KryaToken::Kryakva::OPERATOR},
        {".", KryaToken::Kryakva::OPERATOR},
};
static const std::map<std::string_view, KryaToken::Kryakva> ASSIGN_OPERATOR = {
        {":=",  KryaToken::Kryakva::ASSIGN_OPERATOR},
        {"=",  KryaToken::Kryakva::ASSIGN_OPERATOR},
        {"+=", KryaToken::Kryakva::ASSIGN_OPERATOR},
        {"-=", KryaToken::Kryakva::ASSIGN_OPERATOR},
        {"*=", KryaToken::Kryakva::ASSIGN_OPERATOR},
        {"/=", KryaToken::Kryakva::ASSIGN_OPERATOR},
        {"%=", KryaToken::Kryakva::ASSIGN_OPERATOR},
};
static const std::map<std::string_view, KryaToken::Kryakva> BRACES = {
        {"(", KryaToken::Kryakva::OPEN_PARENTHESIS},
        {")", KryaToken::Kryakva::CLOSE_PARENTHESIS},
        {"[", KryaToken::Kryakva::OPEN_PARENTHESIS},
        {"]", KryaToken::Kryakva::CLOSE_PARENTHESIS},
        {"{", KryaToken::Kryakva::OPEN_PARENTHESIS},
        {"}", KryaToken::Kryakva::CLOSE_PARENTHESIS},
};
static const std::map<std::string_view, KryaToken::Kryakva> SEPARATORS = {
        {" ",  KryaToken::Kryakva::SPACE},
        {"\t", KryaToken::Kryakva::TAB},
        {"\n", KryaToken::Kryakva::ENDLINE},
        {";",  KryaToken::Kryakva::SEMICOLON},
        {",",  KryaToken::Kryakva::COMMA},
};
bool is_operator(const std::string_view& signature);
bool is_assign_operator(const std::string_view& signature);
bool is_brace(const std::string_view& signature);
bool is_separator(const std::string_view& signature);
bool is_identifier(const std::string_view& signature);
bool is_string_constant(const std::string_view& signature);
bool is_number_constant(const std::string_view& signature);
std::optional<KryaToken::Kryakva> token_type_read(std::string_view KRYA_KVa);
std::optional<KryaToken> single_krya_read(std::string_view& text);
std::vector<KryaToken> token_read(std::string_view text);
#endif
