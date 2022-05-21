#ifndef DSL_TOKEN_H
#define DSL_TOKEN_H

#include <unordered_map>
#include <vector>
#include <string_view>
#include <regex>

#include <boost/regex.hpp>


struct KryaToken {

    enum class Kryakva {
        VAR_KEYWORD,
        TRUE_KEYWORD,
        FALSE_KEYWORD,



        ADD_OPERATOR,
        SUB_OPERATOR,
        MUL_OPERATOR,
        DIV_OPERATOR,
        MOD_OPERATOR,
        DOT_OPERATOR,

        GTOE_OPERATOR,  // >=
        GT_OPERATOR,    // >
        LT_OPERATOR,    // <
        LTOE_OPERATOR,  // <=
        EQ_OPERATOR,    // ==
        NEQ_OPERATOR,   // !=
        AND_OPERATOR,   // &&
        OR_OPERATOR,    // ||


        //NEW_OPERATOR,
        GET_OPERATOR,
        IS_VALID_OPERATOR,
        HAS_NEXT_OPERATOR,
        HAS_PREV_OPERATOR,
        INSERT_AFTER_OPERATOR,
        INSERT_BEFORE_OPERATOR,
        ERASE_OPERATOR,




        ASSIGN_OPERATOR,

        /*CURLY_OPEN_BRACE,
        CURLY_CLOSE_BRACE,
         */
        OPEN_PARENTHESIS,
        CLOSE_PARENTHESIS,
        //OPEN_SQUARE_BRACE,
        //CLOSE_SQUARE_BRACE,

        NUMBER_CONSTANT,
        STRING_CONSTANT,
        IDENTIFIER,

        ENDLINE,
        SPACE,
        TAB,

        COMMA,
        SEMICOLON,
        UNDEFINED,


        VAR_NAME,

        BASIC_INT,
        BASIC_STRING,
        BASIC_BOOL,
        BASIC_LINKED_LIST,
        BASIC_ITERATOR,

    };

    explicit KryaToken(Kryakva type, std::string_view value = "")
        : type(type), value(std::string(value)){};


    const std::string value;
    Kryakva type;
    int line_number;
};



const std::regex IDENTIFIER_REGEX       ( R"([a-zA-Z_]+)");
const std::regex STRING_CONSTANT_REGEX  ( R"("(\\.|[^\\"])*")" );
const std::regex NUMBER_CONSTANT_REGEX  ( R"([-]?\d+)"        );


const std::unordered_map<std::string_view, KryaToken::Kryakva> KEYWORDS = {
        {"crt",   KryaToken::Kryakva::VAR_KEYWORD},
        {"True",  KryaToken::Kryakva::TRUE_KEYWORD},
        {"False", KryaToken::Kryakva::FALSE_KEYWORD},
};





const std::unordered_map<std::string_view, KryaToken::Kryakva> OPERATORS = {
        {"+",            KryaToken::Kryakva::ADD_OPERATOR},
        {"-",            KryaToken::Kryakva::SUB_OPERATOR},
        {"*",            KryaToken::Kryakva::MUL_OPERATOR},
        {"/",            KryaToken::Kryakva::DIV_OPERATOR},
        {"%",            KryaToken::Kryakva::MOD_OPERATOR},
        {".",            KryaToken::Kryakva::DOT_OPERATOR},

        {"<",            KryaToken::Kryakva::LT_OPERATOR},
        {"<=",           KryaToken::Kryakva::LTOE_OPERATOR},
        {">",            KryaToken::Kryakva::GT_OPERATOR},
        {">=",           KryaToken::Kryakva::GTOE_OPERATOR},
        {"==",           KryaToken::Kryakva::EQ_OPERATOR},
        {"!=",           KryaToken::Kryakva::NEQ_OPERATOR},
        {"&&",           KryaToken::Kryakva::AND_OPERATOR},
        {"||",           KryaToken::Kryakva::OR_OPERATOR},

        //{"New",   KryaToken::Kryakva::NEW_OPERATOR},
        {"Erase",        KryaToken::Kryakva::ERASE_OPERATOR},
        {"InsertBefore", KryaToken::Kryakva::INSERT_BEFORE_OPERATOR},
        {"InsertAfter",  KryaToken::Kryakva::INSERT_AFTER_OPERATOR},
        {"HasPrev",      KryaToken::Kryakva::HAS_PREV_OPERATOR},
        {"HasNext",      KryaToken::Kryakva::HAS_NEXT_OPERATOR},
        {"IsValid",      KryaToken::Kryakva::IS_VALID_OPERATOR},
        {"Get",          KryaToken::Kryakva::GET_OPERATOR},
};

const std::unordered_map<std::string_view, KryaToken::Kryakva> BASIC_DATA_TYPES = {
        {"Int",        KryaToken::Kryakva::BASIC_INT},
        {"String",     KryaToken::Kryakva::BASIC_STRING},
        {"Bool",       KryaToken::Kryakva::BASIC_BOOL},
        {"LinkedList", KryaToken::Kryakva::BASIC_LINKED_LIST},
        {"Iterator",   KryaToken::Kryakva::BASIC_ITERATOR},
};

const std::unordered_map<std::string_view, KryaToken::Kryakva> ASSIGN_OPERATOR = {
        {"=", KryaToken::Kryakva::ASSIGN_OPERATOR},
};

const std::unordered_map<std::string_view, KryaToken::Kryakva> BRACES = {
        {"(", KryaToken::Kryakva::OPEN_PARENTHESIS},
        {")", KryaToken::Kryakva::CLOSE_PARENTHESIS},
};

const std::unordered_map<std::string_view, KryaToken::Kryakva> SEPARATORS = {
        {" ",  KryaToken::Kryakva::SPACE},
        {"\t", KryaToken::Kryakva::TAB},
        {"\n", KryaToken::Kryakva::ENDLINE},
        {";",  KryaToken::Kryakva::SEMICOLON},
        {",",  KryaToken::Kryakva::COMMA},
};


bool IsKeyword(const std::string_view& lexem);
bool IsOperator(const std::string_view& lexem);
bool IsAssignOperator(const std::string_view& lexem);
bool IsBrace(const std::string_view& lexem);
bool IsSeparator(const std::string_view& lexem);
bool IsIdentifier(const std::string_view& lexem);
bool IsStringConstant(const std::string_view& lexem);
bool IsNumberConstant(const std::string_view& lexem);
bool IsBasicDataType(const std::string_view& lexem);

std::optional<KryaToken::Kryakva> GetTokenType(std::string_view lexem);
std::optional<KryaToken> GetNextToken(std::string_view& text);


class TokenStream {
public:


    explicit TokenStream(const std::vector<KryaToken>& token_sequence);
    explicit TokenStream(std::vector<KryaToken>&& token_sequence);

    TokenStream(TokenStream&&) = default;
    TokenStream(const TokenStream&) = delete;
    TokenStream& operator=(const TokenStream&) = delete;


    KryaToken& GetCurrentToken();
    const KryaToken& GetCurrentToken() const;

    bool HasNext() const;
    bool HasCurrent() const;


    const KryaToken& GetNextToken() const;
    KryaToken& GetNextToken();

    void MoveToNextToken();
    void MoveToPrevToken();

    const auto begin() const {
        return tokens_.begin();
    }

    const auto end() const {
        return tokens_.end();
    }
private:
    std::vector<KryaToken> tokens_;
    size_t curr_idx = 0;
};

std::vector<KryaToken> SplitIntoTokens(std::string_view text);
#endif
