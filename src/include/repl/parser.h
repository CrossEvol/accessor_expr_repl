//
// Created by mizuk on 2025/2/22.
//

#ifndef PARSER_H
#define PARSER_H
#include <any>
#include <memory>

class Lexer;

class Parser {
    std::unique_ptr<Lexer> lexer;

public:
    explicit Parser(std::unique_ptr<Lexer> &lexer)
        : lexer(std::move(lexer)) {
    }

    ~Parser();

    std::any parse();
};

#endif //PARSER_H
