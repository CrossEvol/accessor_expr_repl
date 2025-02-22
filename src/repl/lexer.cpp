//
// Created by mizuk on 2025/2/22.
//

#include <stdexcept>
#include <cctype>
#include <vector>
#include <repl/lexer.h>
#include <repl/token.h>

void Lexer::advance() {
    this->pos++;
}

Token Lexer::nextToken() {
    this->advance();
    while (this->pos < this->text.size()) {
        this->skipWhiteSpace();
        if (this->pos >= this->text.size()) {
            return Token(TokenType::EOF_, std::monostate{});
        }

        const auto ch = this->currentChar();
        if (ch == '$') {
            return Token(TokenType::DOLLAR);
        }
        if (ch == '=') {
            return Token(TokenType::EQ);
        }
        if (ch == '{') {
            return Token(TokenType::LEFT_CURLY);
        }
        if (ch == '}') {
            return Token(TokenType::RIGHT_CURLY);
        }
        if (ch == '[') {
            return Token(TokenType::LEFT_SQUARE);
        }
        if (ch == ']') {
            return Token(TokenType::RIGHT_SQUARE);
        }
        if (ch == '@') {
            return Token(TokenType::AT);
        }
        if (ch == '.') {
            return Token(TokenType::DOT);
        }
        if (ch == ',') {
            return Token(TokenType::COMMA);
        }
        if (isalpha(ch)) {
            return this->identifier();
        }
        if (isdigit(ch)) {
            return this->number();
        }
    }

    return Token(TokenType::EOF_);
}

Token Lexer::peekToken() {
    const auto prev_pos = this->pos;
    auto token = this->nextToken();
    this->pos = prev_pos;
    return token;
}

char Lexer::currentChar() const {
    if (this->pos >= this->text.size()) {
        throw std::out_of_range("Attempted to read past end of input");
    }
    return this->text[this->pos];
}

void Lexer::skipWhiteSpace() {
    while (this->pos < this->text.size() && isspace(this->currentChar())) {
        this->advance();
    }
}

Token Lexer::number() {
    auto chars = std::vector<char>();
    while (this->pos < this->text.size() && isdigit(this->currentChar())) {
        chars.push_back(this->currentChar());
        this->advance();
    }
    auto number = std::string(chars.begin(), chars.end());
    return {TokenType::NUMBER, number};
}

Token Lexer::identifier() {
    auto chars = std::vector<char>();
    while (this->pos < this->text.size() && isalnum(this->currentChar())) {
        chars.push_back(this->currentChar());
        this->advance();
    }
    auto identifier = std::string(chars.begin(), chars.end());
    if (identifier == "nil") {
        return Token(TokenType::NIL);
    }
    if (identifier == "print") {
        return Token(TokenType::PRINT);
    }
    return {TokenType::IDENTIFIER, identifier};
}
