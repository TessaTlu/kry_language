//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_VAR_DECLARATION_H
#define KRYALANGUAGE_VAR_DECLARATION_H
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <variant>
#include "../non_terminal/non_terminal.h"
#include "../data_type/data_type.h"
#include "../lvalue/lvalue.h"

class VarDeclaration : public non_terminal {
public:
    VarDeclaration()
            : non_terminal(non_terminal::Type::VAR_DECLARATION) {}

    std::string ToString() const override {
        std::stringstream out;

        out << "VAR_DECLARATION NONTERM: \n"
            << "\n" << value.ToString() << "\n" << type.ToString();

        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {
        if (!stream.HasCurrent()) {
            std::stringstream error;
            error << "Expected VarDeclaration";
            throw std::runtime_error(error.str());
        }

        const auto &current_token = stream.GetCurrentToken();

        if (current_token.type == KryaToken::Kryakva::VAR_KEYWORD) {
            stream.MoveToNextToken();

            value.ParseFrom(stream);
            type.ParseFrom(stream);
        } else {
            std::stringstream error;
            error << "Expected var-declarationr, but have -> " << current_token.value
                  << " on line " << current_token.line_number << std::endl;
            throw std::runtime_error(error.str());
        }
    }

    void GenerateRPN(std::ostream &out) const override {

        out << "CRT ";
        out << value.GetName();
        out << " ";
        type.GenerateRPN(out);
        out << std::endl;

        value.GenerateRPN(out);
    }

private:
    LValue value;
    DataType type;
};


#endif //KRYALANGUAGE_VAR_DECLARATION_H
