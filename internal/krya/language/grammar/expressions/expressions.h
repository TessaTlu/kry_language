//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_EXPRESSIONS_H
#define KRYALANGUAGE_EXPRESSIONS_H
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <variant>
#include <stack>
#include "../non_terminal/non_terminal.h"
#include "../assignable/assignable.h"
#include "../operator/operator.h"


class ValueExpression : public non_terminal {
public:
    ValueExpression()
            : non_terminal(non_terminal::Type::VALUE_EXPRESSION) {}

    std::string ToString() const override {
        std::stringstream out;
        for (const auto &val: result) {
            out << val->ToString() << std::endl;
            out << "=============================" << std::endl;
        }
        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {

        // Алгоритм перевода выражений в ПолИЗ
        using TokType = KryaToken::Kryakva;
        using NtType = non_terminal::Type;
        std::stack<NontermHolder> frames;

        while (stream.HasCurrent()) {

            const auto &current_token = stream.GetCurrentToken();

            switch (current_token.type) {

                case TokType::TRUE_KEYWORD:
                case TokType::FALSE_KEYWORD:
                case TokType::NUMBER_CONSTANT:
                case TokType::STRING_CONSTANT: {
                    NontermHolder rval = MakeNonterminal(NtType::RVALUE);
                    rval->ParseFrom(stream);
                    result.push_back(rval);
                }
                    break;

                case TokType::IDENTIFIER:
                case TokType::VAR_NAME: {
                    NontermHolder lval = MakeNonterminal(NtType::LVALUE);
                    lval->ParseFrom(stream);
                    result.push_back(lval);
                }
                    break;

                case TokType::OPEN_PARENTHESIS: {
                    NontermHolder lbrace = MakeNonterminal(NtType::OPERATOR);
                    lbrace->ParseFrom(stream);
                    frames.push(lbrace);
                }
                    break;

                case TokType::CLOSE_PARENTHESIS: {

                    NontermHolder rbrace = MakeNonterminal(NtType::OPERATOR);
                    rbrace->ParseFrom(stream);

                    while (
                            !frames.empty()
                            && (
                                    std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                    Operator::OpType::OPARENTH
                            )
                            ) {
                        //std::cerr << frames.top()->ToString() << std::endl;
                        result.push_back(frames.top());
                        frames.pop();
                    }


                    if (!frames.empty()
                        && std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                           Operator::OpType::OPARENTH) {
                        frames.pop();
                    }


                }
                    break;

                case TokType::GET_OPERATOR:
                case TokType::ERASE_OPERATOR:
                case TokType::INSERT_BEFORE_OPERATOR:
                case TokType::INSERT_AFTER_OPERATOR: {
                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);
                    frames.push(oper);
                }
                    break;

                case TokType::IS_VALID_OPERATOR:
                case TokType::HAS_NEXT_OPERATOR:
                case TokType::HAS_PREV_OPERATOR: {
                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);
                    result.push_back(oper);
                }
                    break;

                case TokType::MUL_OPERATOR:
                case TokType::DIV_OPERATOR: {

                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);
                    frames.push(oper);
                }
                    break;

                case TokType::ADD_OPERATOR:
                case TokType::SUB_OPERATOR: {

                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);

                    if (frames.empty()
                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                           Operator::OpType::OPARENTH) {
                        frames.push(oper);
                    } else {

                        while (
                                !frames.empty()
                                && (
                                        std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                        Operator::OpType::MUL
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::DIV
                                )
                                ) {
                            //std::cerr << frames.top()->ToString() << std::endl;
                            result.push_back(frames.top());
                            frames.pop();
                        }

                        frames.push(oper);
                    }

                }
                    break;

                case TokType::LTOE_OPERATOR:
                case TokType::LT_OPERATOR:
                case TokType::GT_OPERATOR:
                case TokType::GTOE_OPERATOR: {

                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);

                    if (frames.empty()
                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                           Operator::OpType::OPARENTH) {
                        frames.push(oper);
                    } else {

                        while (
                                !frames.empty()
                                && std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                   Operator::OpType::OPARENTH
                                && (
                                        std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                        Operator::OpType::MUL
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::DIV
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::SUB
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::ADD
                                )
                                ) {
                            //std::cerr << frames.top()->ToString() << std::endl;

                            if (
                                    std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                    Operator::OpType::LTOE
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::LT
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::GT
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::GTOE
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::EQ
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::NEQ

                                    ) {
                                std::stringstream error;
                                error << "INVALID LOGIC EXPRESSION" << std::endl;
                                throw std::runtime_error(error.str());
                            }

                            result.push_back(frames.top());
                            frames.pop();
                        }

                        frames.push(oper);
                    }

                }
                    break;


                case TokType::NEQ_OPERATOR:
                case TokType::EQ_OPERATOR: {
                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);

                    if (frames.empty()
                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                           Operator::OpType::OPARENTH) {
                        frames.push(oper);
                    } else {

                        while (
                                !frames.empty()
                                && std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                   Operator::OpType::OPARENTH
                                && (
                                        std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                        Operator::OpType::MUL
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::DIV
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::SUB
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::ADD
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::LTOE
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::LT
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::GT
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                           Operator::OpType::GTOE

                                )
                                ) {
                            //std::cerr << frames.top()->ToString() << std::endl;

                            if (
                                    std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                    Operator::OpType::LTOE
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::LT
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::GT
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::GTOE
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::EQ
                                    || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                                       Operator::OpType::NEQ
                                    ) {
                                std::stringstream error;
                                error << "INVALID LOGIC EXPRESSION" << std::endl;
                                throw std::runtime_error(error.str());
                            }

                            result.push_back(frames.top());
                            frames.pop();
                        }

                        frames.push(oper);
                    }
                }
                    break;


                case TokType::AND_OPERATOR: {
                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);

                    if (frames.empty()
                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                           Operator::OpType::OPARENTH) {
                        frames.push(oper);
                    } else {

                        while (
                                !frames.empty()
                                && std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                   Operator::OpType::OPARENTH
                                && (
                                        std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                        Operator::OpType::AND
                                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                           Operator::OpType::OR
                                )
                                ) {
                            std::cerr << frames.top()->ToString() << std::endl;

                            result.push_back(frames.top());
                            frames.pop();
                        }

                        frames.push(oper);
                    }

                }
                    break;


                case TokType::OR_OPERATOR: {
                    NontermHolder oper = MakeNonterminal(NtType::OPERATOR);
                    oper->ParseFrom(stream);

                    if (frames.empty()
                        || std::dynamic_pointer_cast<Operator>(frames.top())->GetType() ==
                           Operator::OpType::OPARENTH) {
                        frames.push(oper);
                    } else {

                        while (
                                !frames.empty()
                                && std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                   Operator::OpType::OPARENTH
                                && (
                                        std::dynamic_pointer_cast<Operator>(frames.top())->GetType() !=
                                        Operator::OpType::OR
                                )
                                ) {
                            //std::cerr << frames.top()->ToString() << std::endl;

                            result.push_back(frames.top());
                            frames.pop();
                        }

                        frames.push(oper);
                    }

                }
                    break;


                case TokType::SEMICOLON: {
                    while (!frames.empty()) {
                        result.push_back(frames.top());
                        frames.pop();
                    }
                    stream.MoveToNextToken();
                    return;
                }
                    break;

                default : {
                    std::stringstream out;
                    out << "Invalid token: " << stream.GetCurrentToken().value
                        << " on line -> " << stream.GetCurrentToken().line_number << std::endl;
                    throw std::runtime_error(out.str());
                }


            }

        }

        stream.MoveToNextToken();

    }


    void GenerateRPN(std::ostream &out) const override {

        for (auto &nonterm: result) {
            nonterm->GenerateRPN(out);

        }
    }

private:
    std::vector<NontermHolder> result;
};

class AssignExpression : public non_terminal {
public:
    AssignExpression()
            : non_terminal(non_terminal::Type::ASSIGN_EXPRESSION) {}

    std::string ToString() const override {
        std::stringstream out;

        out << "ASSIGN_EXPRESSION: " << std::endl
            << lhs.ToString() << " " << std::endl
            << rhs.ToString();
        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {

        if (!stream.HasCurrent()) {
            std::stringstream error;
            error << "Expected VarDeclaration";
            throw std::runtime_error(error.str());
        }

        lhs.ParseFrom(stream);

        const auto &assign_token = stream.GetCurrentToken();

        if (assign_token.type != KryaToken::Kryakva::ASSIGN_OPERATOR) {
            std::stringstream error;
            error << "Expected ASSIGN OPERATOR, but have -> "
                  << assign_token.value << " on line " << assign_token.line_number;
            throw std::runtime_error(error.str());
        }

        stream.MoveToNextToken();

        rhs.ParseFrom(stream);
    }


    void GenerateRPN(std::ostream &out) const override {
        lhs.GenerateRPN(out);
        rhs.GenerateRPN(out);
        out << "ASN" << std::endl;
    }

private:
    Assignable lhs;
    ValueExpression rhs;
};


#endif //KRYALANGUAGE_EXPRESSIONS_H
