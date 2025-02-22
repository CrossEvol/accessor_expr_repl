//
// Created by mizuk on 2025/2/22.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>

enum class TokenType {
    DOLLAR,
    EQ,
    LEFT_CURLY,
    RIGHT_CURLY,
    LEFT_SQUARE,
    RIGHT_SQUARE,
    AT,
    PRINT,
    NIL,
    DOT,
    COMMA,
    IDENTIFIER,
    NUMBER,
    EOF_,
};

struct Token {
    TokenType type;
    std::variant<std::monostate, std::string, int> value;

    explicit Token(const TokenType type)
        : Token(type, std::monostate{}) {
    }

    Token(const TokenType type, const std::variant<std::monostate, std::string, int> &value)
        : type(type),
          value(value) {
    }
};

#endif //TOKEN_H
