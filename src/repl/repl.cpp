//
// Created by mizuk on 2025/2/22.
//

#include <iostream>
#include <ostream>
#include <repl/interpreter.h>
#include <repl/repl.h>
#include <repl/lexer.h>
#include <repl/object.h>
#include <repl/parser.h>

#include "fmt/core.h"

[[noreturn]] void REPL::Start() {
    std::cout << "------------------ REPL::Start() ------------------" << std::endl;

    const auto interpreter = std::make_unique<Interpreter>();

    while (true) {
        std::string input_line;
        fmt::print("> "); // Output the prompt

        if (!std::getline(std::cin, input_line)) {
            continue;
        }

        try {
            auto lexer = std::make_unique<Lexer>(input_line);
            const auto parser = std::make_unique<Parser>(lexer);

            auto expr = parser->parse();

            const auto object = interpreter->visit(*expr);
            fmt::println("{}", object->toString());
        } catch (...) {
            fmt::println("ERROR");
        }
    }
}
