//
// Created by mizuk on 2025/2/22.
//

#include <stdexcept>
#include <repl/ast.h>
#include "repl/interpreter.h"
#include <repl/object.h>

void Globals::global_set(const std::string &key, const std::shared_ptr<Object> &value) {
    this->map[key] = value;
}

bool Globals::exists(const std::string &key) const {
    return this->map.contains(key);
}

std::shared_ptr<Object> Globals::global_get(const std::string &key) const {
    if (this->map.contains(key)) {
        return this->map.at(key);
    }
    throw InterpreterError(InterpretErrorType::UNDEFINED_VARIABLE);
}

void Interpreter::global_set(const std::string &key, std::shared_ptr<Object> &value) const {
    this->globals->global_set(key, value);
}

bool Interpreter::exists(const std::string &key) const {
    return this->globals->exists(key);
}

std::shared_ptr<Object> Interpreter::global_get(const std::string &key) const {
    return this->globals->global_get(key);
}

std::shared_ptr<Object> Interpreter::callNothing() {
    std::unique_ptr<Object> nil = std::make_unique<Nil>();
    return nil;
}

std::shared_ptr<Object> Interpreter::visit(const Expr &expr) {
    return expr.accept(this);
}

std::shared_ptr<Object> Interpreter::visit(const NumberLiteral &numberLiteral) {
    std::unique_ptr<Object> number = std::make_unique<Number>(numberLiteral.value);
    return number;
}

std::shared_ptr<Object> Interpreter::visit(const NilLiteral &nilLiteral) {
    std::unique_ptr<Object> nil = std::make_unique<Nil>();
    return nil;
}

std::shared_ptr<Object> Interpreter::visit(const Variable &variable) {
    auto object = this->global_get(variable.name);
    return object;
}

std::shared_ptr<Object> Interpreter::visit(const Symbol &symbol) {
    return std::make_shared<Property>(symbol.name);
}

std::shared_ptr<Object> Interpreter::visit(const Assignment &assignment) {
    const auto variable_name = assignment.lhs->name;
    if (!this->exists(variable_name)) {
        throw InterpreterError(InterpretErrorType::UNDEFINED_VARIABLE);
    }
    auto right = this->visit(*assignment.rhs);
    this->global_set(variable_name, right);
    return right;
}

std::shared_ptr<Object> Interpreter::visit(const Declaration &declaration) {
    const auto variable_name = declaration.lhs->name;
    if (const auto duplicated = this->globals->exists(variable_name); duplicated) {
        throw InterpreterError(InterpretErrorType::DUPLICATE_VARIABLES_DEFINED);
    }
    auto value = this->visit(*declaration.rhs);
    this->global_set(variable_name, value);
    return callNothing();
}

std::shared_ptr<Object> Interpreter::visit(const ArrayDefinition &arrayDefinition) {
    auto objects = std::vector<std::shared_ptr<Object> >();
    for (const auto &element: arrayDefinition.elements) {
        auto object = this->visit(*element);
        objects.push_back(object);
    }
    auto array = std::make_shared<Array>(objects);
    return array;
}

std::shared_ptr<Object> Interpreter::visit(const EntryDefinition &entryDefinition) {
    throw std::logic_error("Not implemented");
}

std::shared_ptr<Object> Interpreter::visit(const MapDefinition &mapDefinition) {
    auto map = std::map<std::string, std::shared_ptr<Object> >();
    for (const auto &entry_definition: mapDefinition.entries) {
        auto keyName = entry_definition->key->name;
        auto value = this->visit(*entry_definition->value);
        map.emplace(keyName, value);
    }
    std::shared_ptr<Map> mapObject = std::make_shared<Map>(map);
    return mapObject;
}

std::shared_ptr<Object> Interpreter::visit(const AccessorExpr &accessor_expr) {
    return accessor_expr.accept(this);
}

std::shared_ptr<Object> Interpreter::visit(const GetterExpr &getter_expr) {
    const auto object = this->visit(*getter_expr.instance);
    const auto property = this->visit(*getter_expr.accessor);
    return object->_get_item(property->identifier());
}

std::shared_ptr<Object> Interpreter::visit(const SetterExpr &setter_expr) {
    const auto object = this->visit(*setter_expr.instance);
    const auto property = this->visit(*setter_expr.accessor);
    auto new_value = this->visit(*setter_expr.value);
    object->_set_item(property->identifier(), new_value);
    return new_value;
}

std::shared_ptr<Object> Interpreter::visit(const PrintStatement &printStatement) {
    auto object = this->visit(*printStatement.rhs);
    fmt::print("{}\n", object->toString());
    return callNothing();
}

std::shared_ptr<Object> Interpreter::visit(const EmptyStatement &emptyStatement) {
    return callNothing();
}

std::shared_ptr<Object> Interpreter::visit(const Self &selfNode) {
    return callNothing();
}
