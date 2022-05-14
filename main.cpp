#include "internal/krya/language/token/KryaToken.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>


std::string text_read(std::istream &input) {
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

int main() {
    std::ifstream input("main.py");
    std::string text = text_read(input);
    for (auto &token: token_read(text)) {
        std::cout
                << std::left
                << std::setw(20)
                << token.value
                << std::left
                << std::setw(10)
                << " with type : "
                << std::left
                << std::setw(8)
                << token.KryaToTheWorld(token.type)
                << std::left
                << std::setw(5)
                << std::endl;
    };
    input.close();
    return 0;
}
