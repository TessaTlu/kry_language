//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_OPERATOR_H
#define KRYALANGUAGE_OPERATOR_H
#include "../non_terminal/non_terminal.h"


class Operator : public non_terminal {
public:

    enum class OpType {
        ADD,
        SUB,
        MUL,
        DIV,

        GT,         // >
        GTOE,       // >=
        LT,         // <
        LTOE,       // <=
        EQ,         // ==
        NEQ,        // !=

        AND,        // &&
        OR,         // ||


        GET,
        IS_VALID,
        HAS_NEXT,
        HAS_PREV,
        INSERT_AFTER,
        INSERT_BEFORE,
        ERASE,

        OPARENTH,
        CPARENTH,
    };

    Operator()
            : non_terminal(non_terminal::Type::OPERATOR) {}

    std::string ToString() const override {

        static const std::unordered_map<OpType, std::string> TYPE_TO_STRING = {
                {OpType::ADD,           "+"},
                {OpType::SUB,           "-"},
                {OpType::MUL,           "*"},
                {OpType::DIV,           "/"},

                {OpType::GT,            ">"},
                {OpType::GTOE,          ">="},
                {OpType::LT,            "<"},
                {OpType::LTOE,          "<="},
                {OpType::EQ,            "=="},
                {OpType::NEQ,           "!="},

                {OpType::AND,           "&&"},
                {OpType::OR,            "||"},

                {OpType::OPARENTH,      "("},
                {OpType::CPARENTH,      ")"},

                {OpType::GET,           "Get"},
                {OpType::IS_VALID,      "IsValid"},
                {OpType::HAS_NEXT,      "HasNext"},
                {OpType::HAS_PREV,      "HasPrev"},
                {OpType::INSERT_AFTER,  "InsertAfter"},
                {OpType::INSERT_BEFORE, "InsertBefore"},
                {OpType::ERASE,         "Erase"},


        };

        std::stringstream out;
        out << "OPERATION NONTERM: VALUE ->" << TYPE_TO_STRING.at(type);
        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {
        const KryaToken &current_token = stream.GetCurrentToken();

        static const std::unordered_map<KryaToken::Kryakva, OpType> TOKEN_TYPE_TO_OP_TYPE = {
                {KryaToken::Kryakva::ADD_OPERATOR,           OpType::ADD},
                {KryaToken::Kryakva::SUB_OPERATOR,           OpType::SUB},
                {KryaToken::Kryakva::MUL_OPERATOR,           OpType::MUL},
                {KryaToken::Kryakva::DIV_OPERATOR,           OpType::DIV},

                {KryaToken::Kryakva::OPEN_PARENTHESIS,       OpType::OPARENTH},
                {KryaToken::Kryakva::CLOSE_PARENTHESIS,      OpType::CPARENTH},

                {KryaToken::Kryakva::LT_OPERATOR,            OpType::LT},
                {KryaToken::Kryakva::LTOE_OPERATOR,          OpType::LTOE},

                {KryaToken::Kryakva::GTOE_OPERATOR,          OpType::GTOE},
                {KryaToken::Kryakva::GT_OPERATOR,            OpType::GT},

                {KryaToken::Kryakva::EQ_OPERATOR,            OpType::EQ},
                {KryaToken::Kryakva::NEQ_OPERATOR,           OpType::NEQ},

                {KryaToken::Kryakva::AND_OPERATOR,           OpType::AND},
                {KryaToken::Kryakva::OR_OPERATOR,            OpType::OR},

                {KryaToken::Kryakva::GET_OPERATOR,           OpType::GET},
                {KryaToken::Kryakva::IS_VALID_OPERATOR,      OpType::IS_VALID},
                {KryaToken::Kryakva::HAS_NEXT_OPERATOR,      OpType::HAS_NEXT},
                {KryaToken::Kryakva::HAS_PREV_OPERATOR,      OpType::HAS_PREV},
                {KryaToken::Kryakva::INSERT_AFTER_OPERATOR,  OpType::INSERT_AFTER},
                {KryaToken::Kryakva::INSERT_BEFORE_OPERATOR, OpType::INSERT_BEFORE},
                {KryaToken::Kryakva::ERASE_OPERATOR,         OpType::ERASE},

        };

        if (TOKEN_TYPE_TO_OP_TYPE.contains(current_token.type)) {
            type = TOKEN_TYPE_TO_OP_TYPE.at(current_token.type);
        } else {
            std::stringstream error;
            error << "Expected operator, but have -> " << current_token.value
                  << " on line " << current_token.line_number << std::endl;
            throw std::runtime_error(error.str());
        }


        stream.MoveToNextToken();
    }

    OpType GetType() const {
        return type;
    }

    void GenerateRPN(std::ostream &out) const override {

        static const std::unordered_map<OpType, std::string> TYPE_TO_RPN = {
                {OpType::ADD,           "ADD"},
                {OpType::SUB,           "SUB"},
                {OpType::MUL,           "MUL"},
                {OpType::DIV,           "DIV"},

                {OpType::GT,            "GT"},
                {OpType::GTOE,          "GTOE"},
                {OpType::LT,            "LT"},
                {OpType::LTOE,          "LTOE"},
                {OpType::EQ,            "EQ"},
                {OpType::NEQ,           "NEQ"},

                {OpType::AND,           "AND"},
                {OpType::OR,            "OR"},


                {OpType::OPARENTH,      ""},
                {OpType::CPARENTH,      ""},

                {OpType::GET,           "GET"},
                {OpType::IS_VALID,      "ISVAL"},
                {OpType::HAS_NEXT,      "HSNXT"},
                {OpType::HAS_PREV,      "HSPRV"},
                {OpType::INSERT_AFTER,  "INSA"},
                {OpType::INSERT_BEFORE, "INSB"},
                {OpType::ERASE,         "ERS"},
        };

        if (type != OpType::OPARENTH && type != OpType::CPARENTH) {
            out << TYPE_TO_RPN.at(type) << " " << std::endl;
        }

    }

private:
    OpType type;
};

bool IsOperator(KryaToken::Kryakva type) {

    static const std::unordered_map<KryaToken::Kryakva, Operator::OpType> TOKEN_TYPE_TO_OP_TYPE = {
            {KryaToken::Kryakva::ADD_OPERATOR,           Operator::OpType::ADD},
            {KryaToken::Kryakva::SUB_OPERATOR,           Operator::OpType::SUB},
            {KryaToken::Kryakva::MUL_OPERATOR,           Operator::OpType::MUL},
            {KryaToken::Kryakva::DIV_OPERATOR,           Operator::OpType::DIV},

            {KryaToken::Kryakva::OPEN_PARENTHESIS,       Operator::OpType::OPARENTH},
            {KryaToken::Kryakva::CLOSE_PARENTHESIS,      Operator::OpType::CPARENTH},

            {KryaToken::Kryakva::LT_OPERATOR,            Operator::OpType::LT},
            {KryaToken::Kryakva::LTOE_OPERATOR,          Operator::OpType::LTOE},

            {KryaToken::Kryakva::GTOE_OPERATOR,          Operator::OpType::GTOE},
            {KryaToken::Kryakva::GT_OPERATOR,            Operator::OpType::GT},

            {KryaToken::Kryakva::EQ_OPERATOR,            Operator::OpType::EQ},
            {KryaToken::Kryakva::NEQ_OPERATOR,           Operator::OpType::NEQ},

            {KryaToken::Kryakva::AND_OPERATOR,           Operator::OpType::AND},
            {KryaToken::Kryakva::OR_OPERATOR,            Operator::OpType::OR},

            {KryaToken::Kryakva::GET_OPERATOR,           Operator::OpType::GET},
            {KryaToken::Kryakva::IS_VALID_OPERATOR,      Operator::OpType::IS_VALID},
            {KryaToken::Kryakva::HAS_NEXT_OPERATOR,      Operator::OpType::HAS_NEXT},
            {KryaToken::Kryakva::HAS_PREV_OPERATOR,      Operator::OpType::HAS_PREV},
            {KryaToken::Kryakva::INSERT_AFTER_OPERATOR,  Operator::OpType::INSERT_AFTER},
            {KryaToken::Kryakva::INSERT_BEFORE_OPERATOR, Operator::OpType::INSERT_BEFORE},
            {KryaToken::Kryakva::ERASE_OPERATOR,         Operator::OpType::ERASE},
    };
    return TOKEN_TYPE_TO_OP_TYPE.contains(type);
}

#endif //KRYALANGUAGE_OPERATOR_H
