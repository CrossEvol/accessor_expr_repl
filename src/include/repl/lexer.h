//
// Created by mizuk on 2025/2/22.
//

#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <utility>

struct Token;

class Lexer {
    std::string text;
    int pos{-1};

public:
    explicit Lexer(std::string text)
        : text(std::move(text)) {
    }

    ~Lexer() = default;

    Token nextToken();

    Token peekToken();

private:
    void advance();

    void skipWhiteSpace();

    [[nodiscard]] char currentChar() const;

    [[nodiscard]] Token number() ;

    [[nodiscard]] Token identifier() ;
};

#endif //LEXER_H
