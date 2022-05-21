//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_ASSIGNABLE_H
#define KRYALANGUAGE_ASSIGNABLE_H
#include "../non_terminal/non_terminal.h"
#include "../var_declaration/var_declaration.h"

class Assignable : public non_terminal {
public:

    Assignable()
            : non_terminal(non_terminal::Type::ASSIGNABLE) {}

    std::string ToString() const override {
        std::stringstream out;

        out << "ASSIGNABLE: " << std::endl;

        if (std::holds_alternative<VarDeclaration>(value_)) {
            out << std::get<VarDeclaration>(value_).ToString();
        } else {
            out << std::get<LValue>(value_).ToString();
        }

        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {
        if (!stream.HasCurrent()) {
            std::stringstream error;
            error << "Expected VarDeclaration";
            throw std::runtime_error(error.str());
        }

        const auto &current_token = stream.GetCurrentToken();

        switch (current_token.type) {
            case KryaToken::Kryakva::IDENTIFIER:
            case KryaToken::Kryakva::VAR_NAME: {
                value_.emplace<LValue>();
                std::get<LValue>(value_).ParseFrom(stream);
            }
                break;
            case KryaToken::Kryakva::VAR_KEYWORD: {
                value_.emplace<VarDeclaration>();
                std::get<VarDeclaration>(value_).ParseFrom(stream);
            }
                break;
            default : {
                std::stringstream error;
                error << "Expected assignable, but have -> " << current_token.value
                      << " on line " << current_token.line_number << std::endl;
                throw std::runtime_error(error.str());
            }
        }
    }

    void GenerateRPN(std::ostream &out) const override {
        if (std::holds_alternative<VarDeclaration>(value_)) {
            std::get<VarDeclaration>(value_).GenerateRPN(out);
        } else {
            std::get<LValue>(value_).GenerateRPN(out);
        }
    }

private:
    std::variant<LValue, VarDeclaration> value_;
};


#endif //KRYALANGUAGE_ASSIGNABLE_H
