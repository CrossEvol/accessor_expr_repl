// Tests for the REPL Lexer
// Created by mizuk on 2025/2/22.

#include <gtest/gtest.h>
#include <repl/lexer.h>
#include <repl/token.h>
#include <string>
#include <variant>

using std::string;

TEST(LexerTest, SingleCharacterTokens) {
    // This test checks that each single-character token is recognized correctly.
    string input = "$={}[]@.,";
    Lexer lexer(input);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::DOLLAR);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::EQ);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::LEFT_CURLY);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::RIGHT_CURLY);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::LEFT_SQUARE);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::RIGHT_SQUARE);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::AT);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::DOT);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::COMMA);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::EOF_);
}

TEST(LexerTest, NumberToken) {
    // This test verifies that a sequence of digits is read as a NUMBER token.
    string input = "98765";
    Lexer lexer(input);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::NUMBER);
    // The number token value is stored as a string.
    auto numValue = std::get<int>(token.value);
    EXPECT_EQ(numValue, 98765);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::EOF_);
}

TEST(LexerTest, IdentifierToken) {
    // This test verifies that an alphabetic sequence is read as an IDENTIFIER token.
    string input = "helloWorld";
    Lexer lexer(input);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::IDENTIFIER);
    auto idValue = std::get<std::string>(token.value);
    EXPECT_EQ(idValue, "helloWorld");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::EOF_);
}

TEST(LexerTest, SkipsWhiteSpace) {
    // This test ensures the lexer correctly skips white space between tokens.
    string input = "   $   token  123   ";
    Lexer lexer(input);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::DOLLAR);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::IDENTIFIER);
    auto idValue = std::get<std::string>(token.value);
    EXPECT_EQ(idValue, "token");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::NUMBER);
    auto numValue = std::get<int>(token.value);
    EXPECT_EQ(numValue, 123);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::EOF_);
}

TEST(LexerTest, PeekTokenDoesNotAdvance) {
    // This test checks that calling peekToken() returns the next token without consuming it.
    string input = "$token";
    Lexer lexer(input);

    Token peeked = lexer.peekToken();
    Token first = lexer.nextToken();
    EXPECT_EQ(peeked.type, first.type);
    // Since the first token should be '$', no value is expected.

    // After consuming the '$' token, the next token should be the identifier "token".
    Token second = lexer.nextToken();
    EXPECT_EQ(second.type, TokenType::IDENTIFIER);
    auto idValue = std::get<std::string>(second.value);
    EXPECT_EQ(idValue, "token");

    // Finally, the lexer should return EOF after consuming all tokens.
    Token third = lexer.nextToken();
    EXPECT_EQ(third.type, TokenType::EOF_);
}

TEST(LexerTest, KeywordTokens) {
    string input = "nil print";
    Lexer lexer(input);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::NIL);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::PRINT);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::EOF_);
}

TEST(LexerTest, EmptyInput) {
    string input = "";
    Lexer lexer(input);

    Token token = lexer.nextToken();
    EXPECT_EQ(token.type, TokenType::EOF_);
}
