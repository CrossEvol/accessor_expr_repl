#include <gtest/gtest.h>
#include <repl/parser.h>
#include <repl/lexer.h>
#include <repl/ast.h>

class ParserTest : public ::testing::Test {
protected:
    static std::unique_ptr<Expr> parse(const std::string &input) {
        auto lexer = std::make_unique<Lexer>(input);
        auto parser = std::make_unique<Parser>(lexer);
        return parser->parse();
    }
};

TEST_F(ParserTest, ParsesSimpleNumber) {
    auto expr = parse("42");
    auto expected = std::make_unique<NumberLiteral>(42);
    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesNil) {
    auto expr = parse("nil");
    auto expected = std::make_unique<NilLiteral>();
    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesVariable) {
    auto expr = parse("foo");

    auto expected = std::make_unique<Variable>("foo");

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesDeclaration) {
    auto expr = parse("$x = 42");

    auto var = std::make_unique<Variable>("x");
    std::unique_ptr<Expr> num = std::make_unique<NumberLiteral>(42);
    auto expected = std::make_unique<Declaration>(var, num);

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesPropertyAccess) {
    auto expr = parse("a.b.c");

    std::unique_ptr<Expr> a = std::make_unique<Variable>("a");
    std::unique_ptr<Expr> b = std::make_unique<Symbol>("b");
    std::unique_ptr<Expr> a_b = std::make_unique<AccessorExpr>(a, b);
    std::unique_ptr<Expr> c = std::make_unique<Symbol>("c");
    auto expected = std::make_unique<AccessorExpr>(a_b, c);

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesArrayDefinition) {
    auto expr = parse("@[1, 2, 3,]");

    std::vector<std::unique_ptr<Expr> > elements;
    elements.push_back(std::make_unique<NumberLiteral>(1));
    elements.push_back(std::make_unique<NumberLiteral>(2));
    elements.push_back(std::make_unique<NumberLiteral>(3));

    auto expected = std::make_unique<ArrayDefinition>(std::move(elements));

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesMapDefinition) {
    auto expr = parse("@{x = 1, y = 2,}");

    std::vector<std::unique_ptr<EntryDefinition> > entries;

    auto key1 = std::make_unique<Symbol>("x");
    std::unique_ptr<Expr> value1 = std::make_unique<NumberLiteral>(1);
    entries.push_back(std::make_unique<EntryDefinition>(key1, value1));

    auto key2 = std::make_unique<Symbol>("y");
    std::unique_ptr<Expr> value2 = std::make_unique<NumberLiteral>(2);
    entries.push_back(std::make_unique<EntryDefinition>(key2, value2));

    auto expected = std::make_unique<MapDefinition>(std::move(entries));

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesPrintStatement) {
    auto expr = parse("print 42");

    auto num = std::make_unique<NumberLiteral>(42);
    auto expected = std::make_unique<PrintStatement>(std::move(num));

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesAssignment) {
    auto expr = parse("x = 42");

    auto var = std::make_unique<Variable>("x");
    std::unique_ptr<Expr> rhs = std::make_unique<NumberLiteral>(42);

    auto expected = std::make_unique<Assignment>(var, rhs);

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesEmptyStatement) {
    auto expr = parse("");
    auto expected = std::make_unique<EmptyStatement>();
    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesArrayAccess) {
    auto expr = parse("a[1][2]");

    std::unique_ptr<Expr> a = std::make_unique<Variable>("a");
    std::unique_ptr<Expr> one = std::make_unique<NumberLiteral>(1);
    std::unique_ptr<Expr> a_1 = std::make_unique<AccessorExpr>(a, one);
    std::unique_ptr<Expr> two = std::make_unique<NumberLiteral>(2);
    auto expected = std::make_unique<GetterExpr>(a_1, two);

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesMixedPropertyAndArrayAccess) {
    auto expr = parse("a.b[1].c.d[2]");

    std::unique_ptr<Expr> a = std::make_unique<Variable>("a");
    std::unique_ptr<Expr> b = std::make_unique<Symbol>("b");
    std::unique_ptr<Expr> a_b = std::make_unique<AccessorExpr>(a, b);
    std::unique_ptr<Expr> one = std::make_unique<NumberLiteral>(1);
    std::unique_ptr<Expr> a_b_1 = std::make_unique<AccessorExpr>(a_b, one);
    std::unique_ptr<Expr> c = std::make_unique<Symbol>("c");
    std::unique_ptr<Expr> a_b_1_c = std::make_unique<AccessorExpr>(a_b_1, c);
    std::unique_ptr<Expr> d = std::make_unique<Symbol>("d");
    std::unique_ptr<Expr> a_b_1_c_d = std::make_unique<AccessorExpr>(a_b_1_c, d);
    std::unique_ptr<Expr> two = std::make_unique<NumberLiteral>(2);
    auto expected = std::make_unique<GetterExpr>(a_b_1_c_d, two);

    EXPECT_TRUE(*expr == *expected);
}


TEST_F(ParserTest, ParsesChainedAssignments) {
    auto expr = parse("a = b = c = 1");

    // Build the expected AST from inside out:
    // First create the innermost assignment (c = 1)
    auto c_var = std::make_unique<Variable>("c");
    std::unique_ptr<Expr> num = std::make_unique<NumberLiteral>(1);
    std::unique_ptr<Expr> c_assign = std::make_unique<Assignment>(c_var, num);

    // Then b = (c = 1)
    auto b_var = std::make_unique<Variable>("b");
    std::unique_ptr<Expr> b_assign = std::make_unique<Assignment>(b_var, c_assign);

    // Finally a = (b = (c = 1))
    auto a_var = std::make_unique<Variable>("a");
    auto expected = std::make_unique<Assignment>(a_var, b_assign);

    EXPECT_TRUE(*expr == *expected);
}

TEST_F(ParserTest, ParsesChainedAccessorAssignments) {
    auto expr = parse("a.b = c.d = e[0] = 1");

    // Build the expected AST from inside out
    // First create e[0]
    std::unique_ptr<Expr> e_var = std::make_unique<Variable>("e");
    std::unique_ptr<Expr> zero = std::make_unique<NumberLiteral>(0);
    auto e_accessor = std::make_unique<AccessorExpr>(e_var, zero);

    // Create e[0] = 1
    std::unique_ptr<Expr> one = std::make_unique<NumberLiteral>(1);
    std::unique_ptr<Expr> e_setter = std::make_unique<SetterExpr>(e_accessor->instance,
                                                                  e_accessor->accessor,
                                                                  one);


    // Create c.d
    std::unique_ptr<Expr> c_var = std::make_unique<Variable>("c");
    std::unique_ptr<Expr> d_sym = std::make_unique<Symbol>("d");
    auto c_accessor = std::make_unique<AccessorExpr>(c_var, d_sym);

    // Create c.d = e[0] = 1
    std::unique_ptr<Expr> c_setter = std::make_unique<SetterExpr>(c_accessor->instance,
                                                                  c_accessor->accessor,
                                                                  e_setter);


    // Create a.b
    std::unique_ptr<Expr> a_var = std::make_unique<Variable>("a");
    std::unique_ptr<Expr> b_sym = std::make_unique<Symbol>("b");
    auto a_accessor = std::make_unique<AccessorExpr>(a_var, b_sym);

    // Finally create a.b = c.d = e[0] = 1
    auto expected = std::make_unique<SetterExpr>(a_accessor->instance,
                                                 a_accessor->accessor,
                                                 c_setter);


    EXPECT_TRUE(*expr == *expected);
}
