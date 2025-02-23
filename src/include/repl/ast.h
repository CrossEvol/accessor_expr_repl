//
// Created by mizuk on 2025/2/22.
//

#ifndef AST_H
#define AST_H
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Expr {
public:
    virtual ~Expr() = default;

    virtual bool operator==(const Expr &other) const = 0;

    bool operator!=(const Expr &other) const;
};

class Literal : public Expr {
};

class NumberLiteral final : public Literal {
public:
    int value{};

    explicit NumberLiteral(const int value) : value(value) {
    }

    bool operator==(const Expr &other) const override;
};

class NilLiteral final : public Literal {
public:
    bool operator==(const Expr &other) const override;
};

class Variable final : public Expr {
public:
    std::string name;

    explicit Variable(std::string name) : name(std::move(name)) {
    }

    ~Variable() override = default;

    bool operator==(const Expr &other) const override;
};

class Symbol final : public Expr {
public:
    std::string name;

    explicit Symbol(std::string name) : name(std::move(name)) {
    }

    bool operator==(const Expr &other) const override;
};

class Assignment final : public Expr {
public:
    std::unique_ptr<Expr> lhs;
    std::unique_ptr<Expr> rhs;

    explicit Assignment(std::unique_ptr<Expr> &lhs, std::unique_ptr<Expr> &rhs)
        : lhs(std::move(lhs)), rhs(std::move(rhs)) {
    }

    bool operator==(const Expr &other) const override;
};

class Declaration final : public Expr {
public:
    std::unique_ptr<Variable> lhs;
    std::unique_ptr<Expr> rhs;

    explicit Declaration(std::unique_ptr<Variable> &lhs, std::unique_ptr<Expr> &rhs): lhs(std::move(lhs)),
        rhs(std::move(rhs)) {
    }

    bool operator==(const Expr &other) const override;
};

class ArrayDefinition final : public Expr {
public:
    std::vector<std::unique_ptr<Expr> > elements;

    explicit ArrayDefinition(std::vector<std::unique_ptr<Expr> > elements): elements(std::move(elements)) {
    }

    bool operator==(const Expr &other) const override;
};

class EntryDefinition final : public Expr {
public:
    std::unique_ptr<Symbol> key;
    std::unique_ptr<Expr> value;

    EntryDefinition(std::unique_ptr<Symbol> &key, std::unique_ptr<Expr> &value)
        : key(std::move(key)),
          value(std::move(value)) {
    }

    bool operator==(const Expr &other) const override;
};

class MapDefinition final : public Expr {
public:
    std::vector<std::unique_ptr<EntryDefinition> > entries;

    explicit MapDefinition(std::vector<std::unique_ptr<EntryDefinition> > entries): entries(std::move(entries)) {
    }

    bool operator==(const Expr &other) const override;
};

class AccessorExpr final : public Expr {
public:
    std::unique_ptr<Expr> instance;
    std::unique_ptr<Expr> accessor;

    AccessorExpr(std::unique_ptr<Expr> &instance, std::unique_ptr<Expr> &accessor)
        : instance(std::move(instance)),
          accessor(std::move(accessor)) {
    }

    bool operator==(const Expr &other) const override;
};

class PrintStatement final : public Expr {
public:
    std::unique_ptr<Expr> rhs;

    explicit PrintStatement(std::unique_ptr<Expr> rhs): rhs(std::move(rhs)) {
    }

    bool operator==(const Expr &other) const override;
};

class EmptyStatement final : public Expr {
public:
    bool operator==(const Expr &other) const override;
};

class Self final : public Expr {
public:
    bool operator==(const Expr &other) const override;
};

#endif //AST_H
