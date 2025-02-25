#include <gtest/gtest.h>
#include <memory>
#include <repl/ast.h>
#include <repl/interpreter.h>
#include <repl/object.h>

class InterpreterTest : public ::testing::Test {
protected:
    std::unique_ptr<Interpreter> interpreter;

    void SetUp() override {
        interpreter = std::make_unique<Interpreter>();
    }
};

TEST_F(InterpreterTest, EvaluateNumberLiteral) {
    auto number = std::make_unique<NumberLiteral>(42);
    auto result = interpreter->visit(*number);

    // Check if result is a Number object
    auto *num_obj = dynamic_cast<Number *>(result.get());
    ASSERT_NE(num_obj, nullptr);
    EXPECT_EQ(num_obj->num, 42);
}

TEST_F(InterpreterTest, EvaluateNilLiteral) {
    auto nil = std::make_unique<NilLiteral>();
    auto result = interpreter->visit(*nil);

    // Check if result is a Nil object
    auto *nil_obj = dynamic_cast<Nil *>(result.get());
    ASSERT_NE(nil_obj, nullptr);
}

TEST_F(InterpreterTest, EvaluateVariableDeclaration) {
    auto var_name = std::make_unique<Variable>("x");
    std::unique_ptr<Expr> value = std::make_unique<NumberLiteral>(42);
    auto declaration = std::make_unique<Declaration>(var_name, value);

    interpreter->visit(*declaration);

    // Now try to access the variable
    auto var_access = std::make_unique<Variable>("x");
    auto result = interpreter->visit(*var_access);

    auto *num_obj = dynamic_cast<Number *>(result.get());
    ASSERT_NE(num_obj, nullptr);
    EXPECT_EQ(num_obj->num, 42);
}

TEST_F(InterpreterTest, ThrowsOnUndefinedVariable) {
    auto undefined_var = std::make_unique<Variable>("undefined");
    EXPECT_THROW({
                 interpreter->visit(*undefined_var);
                 }, InterpreterError);
}

TEST_F(InterpreterTest, ThrowsOnDuplicateDeclaration) {
    auto var_name1 = std::make_unique<Variable>("x");
    std::unique_ptr<Expr> value1 = std::make_unique<NumberLiteral>(42);
    auto declaration1 = std::make_unique<Declaration>(var_name1, value1);

    interpreter->visit(*declaration1);

    auto var_name2 = std::make_unique<Variable>("x");
    std::unique_ptr<Expr> value2 = std::make_unique<NumberLiteral>(24);
    auto declaration2 = std::make_unique<Declaration>(var_name2, value2);

    EXPECT_THROW({
                 interpreter->visit(*declaration2);
                 }, InterpreterError);
}

TEST_F(InterpreterTest, EvaluateArrayDefinition) {
    std::vector<std::unique_ptr<Expr> > elements;
    elements.push_back(std::make_unique<NumberLiteral>(1));
    elements.push_back(std::make_unique<NumberLiteral>(2));
    elements.push_back(std::make_unique<NumberLiteral>(3));

    auto array_def = std::make_unique<ArrayDefinition>(std::move(elements));
    auto result = interpreter->visit(*array_def);

    auto *array_obj = dynamic_cast<Array *>(result.get());
    ASSERT_NE(array_obj, nullptr);
    EXPECT_EQ(array_obj->elements.size(), 3);

    auto *first_num = dynamic_cast<Number *>(array_obj->elements[0].get());
    ASSERT_NE(first_num, nullptr);
    EXPECT_EQ(first_num->num, 1);
}

TEST_F(InterpreterTest, EvaluateMapDefinition) {
    std::vector<std::unique_ptr<EntryDefinition> > entries;

    auto key1 = std::make_unique<Symbol>("name");
    std::unique_ptr<Expr> value1 = std::make_unique<NumberLiteral>(42);
    entries.push_back(std::make_unique<EntryDefinition>(key1, value1));

    auto map_def = std::make_unique<MapDefinition>(std::move(entries));
    auto result = interpreter->visit(*map_def);

    auto *map_obj = dynamic_cast<Map *>(result.get());
    ASSERT_NE(map_obj, nullptr);
    EXPECT_EQ(map_obj->map.size(), 1);

    auto *value = dynamic_cast<Number *>(map_obj->map["name"].get());
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(value->num, 42);
}

TEST_F(InterpreterTest, ArrayAccessor) {
    // Create and store an array
    std::vector<std::unique_ptr<Expr> > elements;
    elements.push_back(std::make_unique<NumberLiteral>(1));
    elements.push_back(std::make_unique<NumberLiteral>(2));

    auto var_name = std::make_unique<Variable>("arr");
    std::unique_ptr<Expr> array_def = std::make_unique<ArrayDefinition>(std::move(elements));
    auto declaration = std::make_unique<Declaration>(var_name, array_def);

    interpreter->visit(*declaration);

    // Create getter expression
    std::unique_ptr<Expr> instance = std::make_unique<Variable>("arr");
    std::unique_ptr<Expr> accessor = std::make_unique<NumberLiteral>(1);
    auto getter = std::make_unique<GetterExpr>(instance, accessor);

    auto result = interpreter->visit(*getter);
    auto *num_obj = dynamic_cast<Number *>(result.get());
    ASSERT_NE(num_obj, nullptr);
    EXPECT_EQ(num_obj->num, 2);
}

TEST_F(InterpreterTest, MapAccessor) {
    // Create and store a map
    std::vector<std::unique_ptr<EntryDefinition> > entries;
    auto key1 = std::make_unique<Symbol>("age");
    std::unique_ptr<Expr> value1 = std::make_unique<NumberLiteral>(25);
    entries.push_back(std::make_unique<EntryDefinition>(key1, value1));

    auto var_name = std::make_unique<Variable>("person");
    std::unique_ptr<Expr> map_def = std::make_unique<MapDefinition>(std::move(entries));
    auto declaration = std::make_unique<Declaration>(var_name, map_def);

    interpreter->visit(*declaration);

    // Create getter expression
    std::unique_ptr<Expr> instance = std::make_unique<Variable>("person");
    std::unique_ptr<Expr> accessor = std::make_unique<Symbol>("age");
    auto getter = std::make_unique<GetterExpr>(instance, accessor);

    auto result = interpreter->visit(*getter);
    auto *num_obj = dynamic_cast<Number *>(result.get());
    ASSERT_NE(num_obj, nullptr);
    EXPECT_EQ(num_obj->num, 25);
}

TEST_F(InterpreterTest, EvaluateAssignment) {
    // First declare a variable
    auto var_name = std::make_unique<Variable>("x");
    std::unique_ptr<Expr> initial_value = std::make_unique<NumberLiteral>(42);
    auto declaration = std::make_unique<Declaration>(var_name, initial_value);
    interpreter->visit(*declaration);

    // Then create and evaluate an assignment
    auto var_name_assign = std::make_unique<Variable>("x");
    std::unique_ptr<Expr> new_value = std::make_unique<NumberLiteral>(100);
    auto assignment = std::make_unique<Assignment>(var_name_assign, new_value);
    auto result = interpreter->visit(*assignment);

    // Check the result of the assignment
    auto *num_obj = dynamic_cast<Number *>(result.get());
    ASSERT_NE(num_obj, nullptr);
    EXPECT_EQ(num_obj->num, 100);

    // Verify the variable was actually updated
    auto var_access = std::make_unique<Variable>("x");
    auto stored_value = interpreter->visit(*var_access);
    auto *stored_num = dynamic_cast<Number *>(stored_value.get());
    ASSERT_NE(stored_num, nullptr);
    EXPECT_EQ(stored_num->num, 100);
}

TEST_F(InterpreterTest, AssignmentThrowsOnUndefinedVariable) {
    auto var_name = std::make_unique<Variable>("undefined");
    std::unique_ptr<Expr> value = std::make_unique<NumberLiteral>(42);
    auto assignment = std::make_unique<Assignment>(var_name, value);

    EXPECT_THROW({
        interpreter->visit(*assignment);
    }, InterpreterError);
}

TEST_F(InterpreterTest, ArraySetterExpr) {
    // First create and store an array
    std::vector<std::unique_ptr<Expr>> elements;
    elements.push_back(std::make_unique<NumberLiteral>(1));
    elements.push_back(std::make_unique<NumberLiteral>(2));

    auto var_name = std::make_unique<Variable>("arr");
    std::unique_ptr<Expr> array_def = std::make_unique<ArrayDefinition>(std::move(elements));
    auto declaration = std::make_unique<Declaration>(var_name, array_def);
    interpreter->visit(*declaration);

    // Create setter expression
    std::unique_ptr<Expr> instance = std::make_unique<Variable>("arr");
    std::unique_ptr<Expr> accessor = std::make_unique<NumberLiteral>(1);
    std::unique_ptr<Expr> new_value = std::make_unique<NumberLiteral>(42);
    auto setter = std::make_unique<SetterExpr>(instance, accessor, new_value);

    // Test the setter
    auto result = interpreter->visit(*setter);
    auto *num_obj = dynamic_cast<Number *>(result.get());
    ASSERT_NE(num_obj, nullptr);
    EXPECT_EQ(num_obj->num, 42);

    // Verify the array was actually updated
    std::unique_ptr<Expr> get_instance = std::make_unique<Variable>("arr");
    std::unique_ptr<Expr> get_accessor = std::make_unique<NumberLiteral>(1);
    auto getter = std::make_unique<GetterExpr>(get_instance, get_accessor);
    auto stored_value = interpreter->visit(*getter);
    auto *stored_num = dynamic_cast<Number *>(stored_value.get());
    ASSERT_NE(stored_num, nullptr);
    EXPECT_EQ(stored_num->num, 42);
}

TEST_F(InterpreterTest, MapSetterExpr) {
    // First create and store a map
    std::vector<std::unique_ptr<EntryDefinition>> entries;
    auto key1 = std::make_unique<Symbol>("age");
    std::unique_ptr<Expr> value1 = std::make_unique<NumberLiteral>(25);
    entries.push_back(std::make_unique<EntryDefinition>(key1, value1));

    auto var_name = std::make_unique<Variable>("person");
    std::unique_ptr<Expr> map_def = std::make_unique<MapDefinition>(std::move(entries));
    auto declaration = std::make_unique<Declaration>(var_name, map_def);
    interpreter->visit(*declaration);

    // Create setter expression
    std::unique_ptr<Expr> instance = std::make_unique<Variable>("person");
    std::unique_ptr<Expr> accessor = std::make_unique<Symbol>("age");
    std::unique_ptr<Expr> new_value = std::make_unique<NumberLiteral>(30);
    auto setter = std::make_unique<SetterExpr>(instance, accessor, new_value);

    // Test the setter
    auto result = interpreter->visit(*setter);
    auto *num_obj = dynamic_cast<Number *>(result.get());
    ASSERT_NE(num_obj, nullptr);
    EXPECT_EQ(num_obj->num, 30);

    // Verify the map was actually updated
    std::unique_ptr<Expr> get_instance = std::make_unique<Variable>("person");
    std::unique_ptr<Expr> get_accessor = std::make_unique<Symbol>("age");
    auto getter = std::make_unique<GetterExpr>(get_instance, get_accessor);
    auto stored_value = interpreter->visit(*getter);
    auto *stored_num = dynamic_cast<Number *>(stored_value.get());
    ASSERT_NE(stored_num, nullptr);
    EXPECT_EQ(stored_num->num, 30);
}
