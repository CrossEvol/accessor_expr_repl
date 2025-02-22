//
// Created by mizuk on 2025/2/22.
//

#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <utility>

class Lexer {
    std::string text;
    unsigned int pos{};

public:
    explicit Lexer(std::string text)
        : text(std::move(text)) {
    }

    ~Lexer() = default;

    void advance();
};

#endif //LEXER_H
