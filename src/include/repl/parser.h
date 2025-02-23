//
// Created by mizuk on 2025/2/22.
//

#ifndef PARSER_H
#define PARSER_H
#include <memory>

#include <stdexcept>
#include "repl/token.h"
#include "repl/lexer.h"
#include "repl/ast.h"

class Parser {
    std::unique_ptr<Lexer> lexer;
    Token current_token;
    std::variant<std::unique_ptr<Expr>, std::monostate> prev_lhs;

public:
    explicit Parser(std::unique_ptr<Lexer> &lexer)
        : lexer(std::move(lexer)), current_token(Token(TokenType::EOF_)), prev_lhs(std::monostate()) {
    }

    ~Parser() = default;

    void consume(TokenType type);

    [[nodiscard]] Token peek() const;

    /*
     *expr : declaration | assign_expr | rhs ;
     *
     *rhs : accessor_expr | definition | literal ;
     *
     *declaration : DOLLAR variable EQ rhs SEMICOLON
     *
     *assign_expr : accessor_expr EQ rhs
     *
     *definition : map_definition | array_definition
     *
     *map_definition : AT LEFT_CURLY (entry_definition)* RIGHT_CURLY
     *
     *entry_definition : symbol EQ rhs COMMA
     *
     *array_definition: AT LEFT_SQUARE (item_definition)* RIGHT_SQUARE
     *
     *item_definition : rhs COMMA
     *
     *accessor_expr : property_accessor_expr | index_accessor_expr | variable
     *
     *property_accessor_expr : accessor_expr DOT symbol
     *
     *index_accessor_expr : accessor_expr LEFT_SQUARE (variable | number) RIGHT_SQUARE
     *
     *print_stmt : PRINT rhs ;
     *
     *literal : number | null
     *
     *number : NUMBER
     *
     *null : NULL
     *
     *variable : IDENTIFIER
     *
     *symbol : IDENTIFIER
     *
     */
    std::unique_ptr<Expr> parse();

    std::unique_ptr<Expr> expr();

    std::unique_ptr<Declaration> declaration();

    std::unique_ptr<Assignment> assign_expr();

    std::unique_ptr<Expr> definition();

    std::unique_ptr<MapDefinition> map_definition();

    std::unique_ptr<EntryDefinition> entry_definition();

    std::unique_ptr<ArrayDefinition> array_definition();

    std::unique_ptr<AccessorExpr> accessor_expr();

    std::unique_ptr<PrintStatement> print_stmt();

    std::unique_ptr<Expr> rhs();

    std::unique_ptr<Literal> literal();

    std::unique_ptr<NumberLiteral> number();

    std::unique_ptr<NilLiteral> null();

    std::unique_ptr<Variable> variable();

    std::unique_ptr<Symbol> symbol();

    std::unique_ptr<EmptyStatement> empty_stmt();
};

enum class ParseErrorType {
    EXPECT_EXPR_BUT_INDEED_ASSIGN,
    UNKNOWN_LITERAL,
    UNKNOWN_RHS,
    UNKNOWN_DEFINITION,
};

class ParserError final : public std::runtime_error {
public:
    ParseErrorType error_type;

    explicit ParserError(const ParseErrorType &type): std::runtime_error("ParseError!"), error_type(type) {
    }
};

#endif //PARSER_H
