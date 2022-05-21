#pragma once

#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <variant>
#include "../token/token.h"
#include "non_terminal/non_terminal.h"
#include "operator/operator.h"
#include "lvalue/lvalue.h"
#include "rvalue/rvalue.h"
#include "lang/lang.h"
#include "data_type/data_type.h"
#include "var_declaration/var_declaration.h"
#include "assignable/assignable.h"
#include "expressions/expressions.h"


using NontermHolder = std::shared_ptr<non_terminal>;

NontermHolder MakeNonterminal(non_terminal::Type type);

namespace non_terminals {};

NontermHolder MakeNonterminal(non_terminal::Type type) {
    using nt = non_terminal::Type;
    switch (type) {
        case nt::TYPE_SPECIFIER: {
            return std::make_shared<DataType>();
        }
        case nt::VAR_DECLARATION: {
            return std::make_shared<VarDeclaration>();
        }

        case nt::VALUE_EXPRESSION: {
            return std::make_shared<ValueExpression>();
        }

        case nt::LVALUE: {
            return std::make_shared<LValue>();
        }
        case nt::RVALUE: {
            return std::make_shared<RValue>();
        }
        case nt::LANG: {
            return std::make_shared<Lang>();
        }
        case nt::OPERATOR: {
            return std::make_shared<Operator>();
        }
        case nt::ASSIGNABLE: {
            return std::make_shared<Assignable>();
        }
        case nt::ASSIGN_EXPRESSION: {
            return std::make_shared<AssignExpression>();
        }
        default: {
            throw std::runtime_error("undefined type");
        }

    }
}