//
// Created by mizuk on 2025/2/22.
//

#include <stdexcept>
#include <repl/parser.h>

void Parser::consume(const TokenType type) {
    if (this->current_token.type != type) {
        throw std::runtime_error("Unexpected token");
    }
    this->current_token = this->lexer->nextToken();
}

Token Parser::peek() const {
    return this->lexer->peekToken();
}

std::unique_ptr<Expr> Parser::parse() {
    this->current_token = this->lexer->nextToken();
    return this->expr();
}

std::unique_ptr<Expr> Parser::expr() {
    const auto token = this->current_token;
    if (token.type == TokenType::DOLLAR) {
        return this->declaration();
    }
    if (token.type == TokenType::AT) {
        return this->definition();
    }
    if (token.type == TokenType::NUMBER || token.type == TokenType::NIL) {
        return this->literal();
    }
    if (token.type == TokenType::PRINT) {
        return this->print_stmt();
    }
    if (token.type == TokenType::IDENTIFIER) {
        return this->rhs();
    }
    return this->empty_stmt();
}

std::unique_ptr<Declaration> Parser::declaration() {
    this->consume(TokenType::DOLLAR);
    auto lhs = this->variable();
    this->consume(TokenType::EQ);
    auto rhs = this->rhs();
    return std::make_unique<Declaration>(lhs, rhs);
}

std::unique_ptr<Assignment> Parser::assign_expr() {
    throw std::logic_error("Not implemented");
}

std::unique_ptr<Expr> Parser::definition() {
    const auto token = this->peek();
    if (token.type == TokenType::LEFT_CURLY) {
        return this->map_definition();
    }
    if (token.type == TokenType::LEFT_SQUARE) {
        return this->array_definition();
    }
    throw ParserError(ParseErrorType::UNKNOWN_DEFINITION);
}

std::unique_ptr<MapDefinition> Parser::map_definition() {
    this->consume(TokenType::AT);
    auto entries = std::vector<std::unique_ptr<EntryDefinition> >();
    this->consume(TokenType::LEFT_CURLY);
    while (this->current_token.type != TokenType::RIGHT_CURLY) {
        auto entry = this->entry_definition();
        entries.push_back(std::move(entry));
        this->consume(TokenType::COMMA);
    }
    this->consume(TokenType::RIGHT_CURLY);
    return std::make_unique<MapDefinition>(std::move(entries));
}

std::unique_ptr<EntryDefinition> Parser::entry_definition() {
    auto symbol = this->symbol();
    this->consume(TokenType::EQ);
    auto rhs = this->rhs();
    return std::make_unique<EntryDefinition>(symbol, rhs);
}

std::unique_ptr<ArrayDefinition> Parser::array_definition() {
    this->consume(TokenType::AT);
    auto elements = std::vector<std::unique_ptr<Expr> >();
    this->consume(TokenType::LEFT_SQUARE);
    while (this->current_token.type != TokenType::RIGHT_SQUARE) {
        auto element = this->rhs();
        elements.push_back(std::move(element));
        this->consume(TokenType::COMMA);
    }
    this->consume(TokenType::RIGHT_SQUARE);
    return std::make_unique<ArrayDefinition>(std::move(elements));
}

std::unique_ptr<Expr> Parser::accessor_expr() {
    std::unique_ptr<Expr> instance = this->variable();
    auto is_accessor = false;
    while (this->current_token.type == TokenType::DOT || this->current_token.type == TokenType::LEFT_SQUARE) {
        is_accessor = true;
        const auto token_type = this->current_token.type;
        if (token_type == TokenType::DOT) {
            this->consume(TokenType::DOT);
            std::unique_ptr<Expr> accessor = this->symbol();
            instance = std::make_unique<AccessorExpr>(instance, accessor);
        }
        if (token_type == TokenType::LEFT_SQUARE) {
            this->consume(TokenType::LEFT_SQUARE);
            std::unique_ptr<Expr> accessor = this->number();
            instance = std::make_unique<AccessorExpr>(instance, accessor);
            this->consume(TokenType::RIGHT_SQUARE);
        }
    }
    if (this->current_token.type != TokenType::EQ) {
        return instance;
    }
    if (!is_accessor) {
        this->consume(TokenType::EQ);
        auto rhs = this->rhs();
        auto variable = std::make_unique<Variable>(*dynamic_cast<Variable *>(instance.get()));
        return std::make_unique<Assignment>(variable, rhs);
    }

    auto *accessor_ptr = dynamic_cast<AccessorExpr *>(instance.get());
    if (!accessor_ptr) {
        throw std::runtime_error("Expected AccessorExpr");
    }

    this->consume(TokenType::EQ);
    auto value = this->rhs();

    return std::make_unique<SetterExpr>(
        accessor_ptr->instance,
        accessor_ptr->accessor,
        value
    );
}

std::unique_ptr<PrintStatement> Parser::print_stmt() {
    this->consume(TokenType::PRINT);
    auto rhs = this->rhs();
    return std::make_unique<PrintStatement>(std::move(rhs));
}

std::unique_ptr<Expr> Parser::rhs() {
    const auto token = this->current_token;
    if (token.type == TokenType::AT) {
        return this->definition();
    }
    if (token.type == TokenType::NUMBER || token.type == TokenType::NIL) {
        return this->literal();
    }
    if (token.type == TokenType::IDENTIFIER) {
        return this->accessor_expr();
    }
    throw ParserError(ParseErrorType::UNKNOWN_RHS);
}

std::unique_ptr<Literal> Parser::literal() {
    const auto token = this->current_token;
    if (token.type == TokenType::NUMBER) {
        return this->number();
    }
    if (token.type == TokenType::NIL) {
        return this->null();
    }
    throw ParserError(ParseErrorType::UNKNOWN_LITERAL);
}

std::unique_ptr<NumberLiteral> Parser::number() {
    auto token = this->current_token;
    this->consume(TokenType::NUMBER);
    return std::make_unique<NumberLiteral>(std::get<int>(token.value));
}

std::unique_ptr<NilLiteral> Parser::null() {
    this->consume(TokenType::NIL);
    return std::make_unique<NilLiteral>();
}

std::unique_ptr<Variable> Parser::variable() {
    auto token = this->current_token;
    this->consume(TokenType::IDENTIFIER);
    return std::make_unique<Variable>(std::get<std::string>(token.value));
}

std::unique_ptr<Symbol> Parser::symbol() {
    auto token = this->current_token;
    this->consume(TokenType::IDENTIFIER);
    return std::make_unique<Symbol>(std::get<std::string>(token.value));
}

std::unique_ptr<EmptyStatement> Parser::empty_stmt() {
    this->consume(TokenType::EOF_);
    return std::make_unique<EmptyStatement>();
}
