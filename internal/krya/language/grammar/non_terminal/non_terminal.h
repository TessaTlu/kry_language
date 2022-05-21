//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_NON_TERMINAL_H
#define KRYALANGUAGE_NON_TERMINAL_H

#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <variant>
#include "../../token/token.h"

struct non_terminal {

    enum class Type {
        ASSIGN_EXPRESSION,
        VALUE_EXPRESSION,
        VAR_DECLARATION,
        LVALUE,
        RVALUE,
        ASSIGNABLE,
        TYPE_SPECIFIER,
        OPERATOR,
        LANG,
    };

    non_terminal() = default;

    non_terminal(Type type) : type(type) {}

    virtual std::string ToString() const = 0;

    virtual void GenerateRPN(std::ostream &out) const = 0;

    virtual void ParseFrom(TokenStream &stream) = 0;

    virtual ~non_terminal() = default;

    const Type type;
};

using NontermHolder = std::shared_ptr<non_terminal>;

NontermHolder MakeNonterminal(non_terminal::Type type);




#endif //KRYALANGUAGE_NON_TERMINAL_H
