//
// Created by Расим Гаджиев on 21.05.2022.
//

#ifndef KRYALANGUAGE_DATA_TYPE_H
#define KRYALANGUAGE_DATA_TYPE_H

#include "../non_terminal/non_terminal.h"

class DataType : public non_terminal {
public:

    enum class Typename {
        INT,
        STRING,
        BOOL,
        LINKED_LIST,
        ITERATOR,
    };

    DataType(Typename type = Typename::INT)
            : non_terminal(non_terminal::Type::TYPE_SPECIFIER), type(type) {}

    std::string ToString() const override {

        static const std::unordered_map<Typename, std::string> TYPE_TO_STRING = {
                {Typename::INT,         "Int"},
                {Typename::STRING,      "String"},
                {Typename::BOOL,        "Bool"},
                {Typename::LINKED_LIST, "LinkedList"},
                {Typename::ITERATOR,    "Iterator"},

        };

        std::stringstream out;
        out << "TYPE_SPECIFIER NONTERM: VALUE ->" << TYPE_TO_STRING.at(type);
        return out.str();
    }

    void ParseFrom(TokenStream &stream) override {
        if (!stream.HasCurrent()) {
            std::stringstream error;
            error << "Expected TypeSpecifier";
            throw std::runtime_error(error.str());
        }

        const auto &current_token = stream.GetCurrentToken();

        const static std::unordered_map<KryaToken::Kryakva, Typename> types = {
                {KryaToken::Kryakva::BASIC_STRING,      Typename::STRING},
                {KryaToken::Kryakva::BASIC_INT,         Typename::INT},
                {KryaToken::Kryakva::BASIC_BOOL,        Typename::BOOL},
                {KryaToken::Kryakva::BASIC_LINKED_LIST, Typename::LINKED_LIST},
                {KryaToken::Kryakva::BASIC_ITERATOR,    Typename::ITERATOR},
        };

        if (types.contains(current_token.type)) {
            type = types.at(current_token.type);
            stream.MoveToNextToken();
        } else {
            std::stringstream error;
            error << "Expected type-specifier, but have -> " << current_token.value
                  << " on line " << current_token.line_number << std::endl;
            throw std::runtime_error(error.str());
        }

    }


    void GenerateRPN(std::ostream &out) const override {
        static const std::unordered_map<Typename, std::string> TYPE_TO_STRING = {
                {Typename::INT,         "Int"},
                {Typename::STRING,      "String"},
                {Typename::BOOL,        "Bool"},
                {Typename::LINKED_LIST, "LinkedList"},
                {Typename::ITERATOR,    "Iterator"},

        };
        out << TYPE_TO_STRING.at(type);
    }

private:

    Typename type;
};


#endif //KRYALANGUAGE_DATA_TYPE_H
