//
// Created by mizuk on 2025/2/22.
//

#include <repl/ast.h>

bool Expr::operator!=(const Expr& other) const {
    return !(*this == other);
}

bool NumberLiteral::operator==(const Expr& other) const {
    if (const auto* number = dynamic_cast<const NumberLiteral*>(&other)) {
        return value == number->value;
    }
    return false;
}

bool NilLiteral::operator==(const Expr& other) const {
    return dynamic_cast<const NilLiteral*>(&other) != nullptr;
}

bool Variable::operator==(const Expr& other) const {
    if (const auto* variable = dynamic_cast<const Variable*>(&other)) {
        return name == variable->name;
    }
    return false;
}

bool Symbol::operator==(const Expr& other) const {
    if (const auto* symbol = dynamic_cast<const Symbol*>(&other)) {
        return name == symbol->name;
    }
    return false;
}

bool Assignment::operator==(const Expr& other) const {
    if (const auto* assignment = dynamic_cast<const Assignment*>(&other)) {
        return *lhs == *assignment->lhs && *rhs == *assignment->rhs;
    }
    return false;
}

bool Declaration::operator==(const Expr& other) const {
    if (const auto* declaration = dynamic_cast<const Declaration*>(&other)) {
        return *lhs == *declaration->lhs && *rhs == *declaration->rhs;
    }
    return false;
}

bool ArrayDefinition::operator==(const Expr& other) const {
    if (const auto* array = dynamic_cast<const ArrayDefinition*>(&other)) {
        if (elements.size() != array->elements.size()) return false;
        for (auto i = 0; i < elements.size(); i++) {
            if (*elements[i] != *array->elements[i]) return false;
        }
        return true;
    }
    return false;
}

bool EntryDefinition::operator==(const Expr& other) const {
    if (const auto* entry = dynamic_cast<const EntryDefinition*>(&other)) {
        return *key == *entry->key && *value == *entry->value;
    }
    return false;
}

bool MapDefinition::operator==(const Expr& other) const {
    if (const auto* map = dynamic_cast<const MapDefinition*>(&other)) {
        if (entries.size() != map->entries.size()) return false;
        for (auto i = 0; i < entries.size(); i++) {
            if (*entries[i] != *map->entries[i]) return false;
        }
        return true;
    }
    return false;
}

bool AccessorExpr::operator==(const Expr& other) const {
    if (const auto* accessor_expr = dynamic_cast<const AccessorExpr*>(&other)) {
        return *instance == *accessor_expr->instance && 
               *accessor == *accessor_expr->accessor;
    }
    return false;
}

bool PrintStatement::operator==(const Expr& other) const {
    if (const auto* print = dynamic_cast<const PrintStatement*>(&other)) {
        return *rhs == *print->rhs;
    }
    return false;
}

bool EmptyStatement::operator==(const Expr& other) const {
    return dynamic_cast<const EmptyStatement*>(&other) != nullptr;
}

bool Self::operator==(const Expr& other) const {
    return dynamic_cast<const Self*>(&other) != nullptr;
}