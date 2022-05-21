#include <iostream>
#include <fstream>
#include <vector>
#include "../token/token.h"
#include "grammar.h"



std::string GetProgramText(std::istream &input) {
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

void ParseIntoRPN() {
    std::ifstream input("example.rasim");

    if (!input.is_open()) {
        std::cerr << "No FILE!" << std::endl;
        std::terminate();
    }

    std::string text = GetProgramText(input);
    input.close();
    TokenStream stream(SplitIntoTokens(text));

    NontermHolder lang = MakeNonterminal(non_terminal::Type::LANG);
    lang->ParseFrom(stream);

    std::ofstream output("rpn.rpn");
    lang->GenerateRPN(output);
    output.close();
}

