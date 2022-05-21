//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_RVALUE_H
#define KRYALANGUAGE_RVALUE_H
#include "../non_terminal/non_terminal.h"


class RValue : public non_terminal {
public:
    enum class ValType {
        INT_LITERAL,
        BOOL_LITERAL,
        STRING_LITERAL,
    };

    RValue() : non_terminal(non_terminal::Type::RVALUE) {}

    std::string ToString() const override {
        static const std::unordered_map<ValType, std::string> TYPE_TO_STRING = {
                {ValType::INT_LITERAL,    "INTEGER"},
                {ValType::STRING_LITERAL, "STRING"},
                {ValType::BOOL_LITERAL,   "BOOL"},
        };

        std::stringstream out;
        out << "RVALUE NONTERM: TYPE ->" << TYPE_TO_STRING.at(type) << "\t";
        out << "VALUE -> " << value;
        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {
        const auto &current_token = stream.GetCurrentToken();

        switch (current_token.type) {
            case KryaToken::Kryakva::STRING_CONSTANT: {
                type = ValType::STRING_LITERAL;
            }
                break;
            case KryaToken::Kryakva::NUMBER_CONSTANT : {
                type = ValType::INT_LITERAL;
            }
                break;

            case KryaToken::Kryakva::TRUE_KEYWORD:
            case KryaToken::Kryakva::FALSE_KEYWORD: {
                type = ValType::BOOL_LITERAL;
            }
                break;
            default: {
                std::stringstream error;
                error << "Expected rvalue, but have -> " << current_token.value
                      << " on line " << current_token.line_number << std::endl;
                throw std::runtime_error(error.str());
            }
        }

        value = current_token.value;
        stream.MoveToNextToken();
    }

    void GenerateRPN(std::ostream &out) const override {

        static const std::unordered_map<ValType, std::string> TYPE_TO_STRING = {
                {ValType::INT_LITERAL,    "Int"},
                {ValType::STRING_LITERAL, "String"},
                {ValType::BOOL_LITERAL,   "Bool"},
        };

        out << "PUSH " << TYPE_TO_STRING.at(type) << " " << value << std::endl;

    }

private:
    ValType type;;
    std::string value;
};

bool IsRValue(KryaToken::Kryakva type) {
    return
            type == KryaToken::Kryakva::TRUE_KEYWORD
            || type == KryaToken::Kryakva::FALSE_KEYWORD
            || type == KryaToken::Kryakva::NUMBER_CONSTANT
            || type == KryaToken::Kryakva::STRING_CONSTANT;
}


#endif //KRYALANGUAGE_RVALUE_H
