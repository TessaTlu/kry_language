//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_LVALUE_H
#define KRYALANGUAGE_LVALUE_H
#include "../non_terminal/non_terminal.h"

class LValue : public non_terminal {
public:
    LValue() : non_terminal(non_terminal::Type::LVALUE) {}

    std::string ToString() const override {
        std::stringstream out;
        out << "LVALUE NONTERM: NAME ->" << name;
        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {

        if (!stream.HasCurrent()) {
            std::stringstream error;
            error << "Expected LValue";
            throw std::runtime_error(error.str());
        }

        const auto &current_token = stream.GetCurrentToken();

        if (current_token.type == KryaToken::Kryakva::VAR_NAME || current_token.type == KryaToken::Kryakva::IDENTIFIER) {

            name = current_token.value;
            stream.MoveToNextToken();

        } else {
            std::stringstream error;
            error << "Expected LValue, but have ->" << current_token.value
                  << " on line " << current_token.line_number;
            throw std::runtime_error(error.str());
        }
    }

    void GenerateRPN(std::ostream &out) const override {
        out << "PUSH Var " << name << std::endl;
    }

    std::string GetName() const {
        return name;
    }

private:
    std::string name;
};

bool IsLvalue(KryaToken::Kryakva type) {
    type == KryaToken::Kryakva::VAR_NAME
    || type == KryaToken::Kryakva::IDENTIFIER;
}


#endif //KRYALANGUAGE_LVALUE_H
