//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_LANG_H
#define KRYALANGUAGE_LANG_H
#include "../non_terminal/non_terminal.h"

class Lang : public non_terminal {
public:
    Lang() : non_terminal(non_terminal::Type::LANG) {}

    std::string ToString() const override {
        std::stringstream out;

        out << "LANG: " << std::endl
            << "==========================" << std::endl;

        for (auto &expr: expressions_) {
            out << expr->ToString() << std::endl;
            out << "*****************************************" << std::endl;
        }

        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {
        while (stream.HasCurrent()) {
            NontermHolder expr = MakeNonterminal(non_terminal::Type::ASSIGN_EXPRESSION);
            expr->ParseFrom(stream);
            expressions_.push_back(expr);

        }
    }


    void GenerateRPN(std::ostream &out) const override {
        for (const auto &expr: expressions_) {
            expr->GenerateRPN(out);
        }
    }

private:
    std::vector<NontermHolder> expressions_;
};

#endif //KRYALANGUAGE_LANG_H
