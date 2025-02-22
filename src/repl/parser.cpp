//
// Created by mizuk on 2025/2/22.
//

#include <variant>
#include <repl/parser.h>
#include <repl/lexer.h>

Parser::~Parser() = default;

std::any Parser::parse() {
    return  std::monostate{};
}
