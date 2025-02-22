//
// Created by mizuk on 2025/2/22.
//

#include <iostream>
#include <ostream>
#include <repl/repl.h>
#include <repl/lexer.h>
#include <repl/parser.h>

void REPL::Start() {
    std::cout << "REPL::Start()" << std::endl;

    auto lexer = std::make_unique<Lexer>("");
    const auto parser = std::make_unique<Parser>(lexer);

    parser->parse();
}
