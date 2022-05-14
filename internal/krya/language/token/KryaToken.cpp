#include "KryaToken.h"
#include <vector>
#include <unordered_map>
#include <string_view>
#include <iostream>
#include <boost/regex.hpp>
bool is_operator(const std::string_view &krya) {
    return OPERATORS.contains(krya);
}
bool is_assign_operator(const std::string_view &krya) {
    return ASSIGN_OPERATOR.contains(krya);
}
bool is_brace(const std::string_view &krya) {
    return BRACES.contains(krya);
}
bool is_separator(const std::string_view &krya) {
    return SEPARATORS.contains(krya);
}
bool is_identifier(const std::string_view &krya) {
    return boost::regex_match(std::string(krya), IDENTIFIER_REGEX);
}
bool is_string_constant(const std::string_view &krya) {
    return boost::regex_match(std::string(krya), STRING_CONSTANT_REGEX);
}
bool is_number_constant(const std::string_view &krya) {
    return boost::regex_match(std::string(krya), NUMBER_CONSTANT_REGEX);
}
std::optional<KryaToken::Kryakva> token_type_read(std::string_view KRYA_KVa) {
    if (is_brace(KRYA_KVa)) {
        return BRACES.at(KRYA_KVa);
    } else if (is_assign_operator(KRYA_KVa)) {
        return ASSIGN_OPERATOR.at(KRYA_KVa);
    } else if (is_separator(KRYA_KVa)) {
        return SEPARATORS.at(KRYA_KVa);
    } else if (is_operator(KRYA_KVa)) {
        return OPERATORS.at(KRYA_KVa);
    } else if (is_identifier(KRYA_KVa)) {
        return KryaToken::Kryakva::IDENTIFIER;
    } else if (is_string_constant(KRYA_KVa)) {
        return KryaToken::Kryakva::STRING_CONSTANT;
    } else if (is_number_constant(KRYA_KVa)) {
        return KryaToken::Kryakva::NUMBER_CONSTANT;
    }
    return std::nullopt;
}
std::optional<KryaToken> single_krya_read(std::string_view& text) {
    enum class State {
        WAITING_FOR_SUCCESS,
        WAITING_FOR_UNSUCCESS,
    };
    State current_state = State::WAITING_FOR_SUCCESS;
    KryaToken::Kryakva last_matched = KryaToken::Kryakva::UNDEFINED;
    for (int j = 1; j <= text.size(); j++) {
        auto current_match = token_type_read(text.substr(0, j));
        if (current_match.has_value()) {
            if (current_state == State::WAITING_FOR_SUCCESS) {
                current_state = State::WAITING_FOR_UNSUCCESS;
            }
            last_matched = current_match.value();
        } else {
            if (current_state == State::WAITING_FOR_UNSUCCESS) {
                KryaToken t(last_matched, text.substr(0, j - 1));
                text.remove_prefix(j - 1);
                return t;
            }
        }
    }
    if (last_matched != KryaToken::Kryakva::UNDEFINED && current_state == State::WAITING_FOR_UNSUCCESS) {
        KryaToken t(last_matched, text);
        text.remove_prefix(text.size());
        return t;
    }
    return std::nullopt;
}

std::vector<KryaToken> token_read(std::string_view text) {
    std::vector<KryaToken> result;
    int line_number = 1;
    do {
        auto token = single_krya_read(text);
        if (token.has_value()) {
            if (token.value().type == KryaToken::Kryakva::ENDLINE) {
                line_number++;
            } else {
                if (token.value().type != KryaToken::Kryakva::TAB && token.value().type != KryaToken::Kryakva::SPACE)
                    result.emplace_back(token.value());
            }
        } else {
            std::cerr << "Problem on line [" << line_number << "]" << std::endl;
            break;
        }
    } while (!text.empty());
    return result;
}

std::ostream &operator<<(std::ostream &out, const KryaToken &t) {
    return out;
}
