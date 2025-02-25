//
// Created by mizuk on 2025/2/22.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>

#include "visitor.h"

class Globals {
    std::map<std::string, std::shared_ptr<Object> > map = {};

public:
    Globals() = default;

    ~Globals() = default;

    void global_set(const std::string &key, const std::shared_ptr<Object> &value);

    [[nodiscard]] bool exists(const std::string &key) const;

    [[nodiscard]] std::shared_ptr<Object> global_get(const std::string &key) const;
};

enum class InterpretErrorType {
    UNDEFINED_VARIABLE,
    DUPLICATE_VARIABLES_DEFINED,
};

class InterpreterError final : public std::runtime_error {
public:
    InterpretErrorType error_type;

    explicit InterpreterError(const InterpretErrorType &type)
        : std::runtime_error("InterpreterError!"),
          error_type(type) {
    }
};

class Interpreter final : public Visitor {
    std::unique_ptr<Globals> globals = std::make_unique<Globals>();

    std::tuple<std::shared_ptr<Object>, std::shared_ptr<Object> > last_accessor_pair;

    void global_set(const std::string &key, std::shared_ptr<Object> &value) const;

    [[nodiscard]] bool exists(const std::string &key) const;

    [[nodiscard]] std::shared_ptr<Object> global_get(const std::string &key) const;

    [[nodiscard]] static std::shared_ptr<Object> callNothing();

public:
    Interpreter() = default;

    std::shared_ptr<Object> visit(const Expr &expr) override;

    // Evaluate a NumberLiteral
    std::shared_ptr<Object> visit(const NumberLiteral &numberLiteral) override;

    // Evaluate a NilLiteral
    std::shared_ptr<Object> visit(const NilLiteral &nilLiteral) override;

    // Lookup a Variable's value
    std::shared_ptr<Object> visit(const Variable &variable) override;

    // Evaluate a Symbol
    std::shared_ptr<Object> visit(const Symbol &symbol) override;

    // Evaluate an assignment operation
    std::shared_ptr<Object> visit(const Assignment &assignment) override;

    // Evaluate a declaration operation
    std::shared_ptr<Object> visit(const Declaration &declaration) override;

    // Evaluate an array definition
    std::shared_ptr<Object> visit(const ArrayDefinition &arrayDefinition) override;

    // Evaluate an entry in a map definition
    std::shared_ptr<Object> visit(const EntryDefinition &entryDefinition) override;

    // Evaluate a map definition
    std::shared_ptr<Object> visit(const MapDefinition &mapDefinition) override;

    // Evaluate an accessor expression
    std::shared_ptr<Object> visit(const AccessorExpr &accessor_expr) override;

    // Evaluate an getter expression
    std::shared_ptr<Object> visit(const GetterExpr &getter_expr) override;

    // Evaluate an getter expression
    std::shared_ptr<Object> visit(const SetterExpr &setter_expr) override;

    // Evaluate a print statement
    std::shared_ptr<Object> visit(const PrintStatement &printStatement) override;

    // Evaluate an empty statement
    std::shared_ptr<Object> visit(const EmptyStatement &emptyStatement) override;

    // Evaluate a self-reference (if applicable)
    std::shared_ptr<Object> visit(const Self &selfNode) override;
};

#endif //INTERPRETER_H
