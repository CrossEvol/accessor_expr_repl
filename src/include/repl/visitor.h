//
// Created by mizuk on 2025/2/24.
//

#ifndef VISITOR_H
#define VISITOR_H
#include <memory>

#include "ast.h"

// Forward declarations for AST node types.
class Expr;
class NumberLiteral;
class NilLiteral;
class Variable;
class Symbol;
class Assignment;
class Declaration;
class ArrayDefinition;
class EntryDefinition;
class MapDefinition;
class AccessorExpr;
class PrintStatement;
class EmptyStatement;
class Self;

class Object;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual std::shared_ptr<Object> visit(const Expr& expr) = 0;

    virtual std::shared_ptr<Object> visit(const NumberLiteral &numberLiteral) = 0;

    virtual std::shared_ptr<Object> visit(const NilLiteral &nilLiteral) = 0;

    virtual std::shared_ptr<Object> visit(const Variable &variable) = 0;

    virtual std::shared_ptr<Object> visit(const Symbol &symbol) = 0;

    virtual std::shared_ptr<Object> visit(const Assignment &assignment) = 0;

    virtual std::shared_ptr<Object> visit(const Declaration &declaration) = 0;

    virtual std::shared_ptr<Object> visit(const ArrayDefinition &arrayDefinition) = 0;

    virtual std::shared_ptr<Object> visit(const EntryDefinition &entryDefinition) = 0;

    virtual std::shared_ptr<Object> visit(const MapDefinition &mapDefinition) = 0;

    virtual std::shared_ptr<Object> visit(const AccessorExpr &accessorExpr) = 0;

    virtual std::shared_ptr<Object> visit(const GetterExpr &getter_expr) = 0;

    virtual std::shared_ptr<Object> visit(const SetterExpr &setter_expr) = 0;

    virtual std::shared_ptr<Object> visit(const PrintStatement &printStatement) = 0;

    virtual std::shared_ptr<Object> visit(const EmptyStatement &emptyStatement) = 0;

    virtual std::shared_ptr<Object> visit(const Self &selfNode) = 0;
};

#endif //VISITOR_H
