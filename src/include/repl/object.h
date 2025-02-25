//
// Created by mizuk on 2025/2/22.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <map>
#include <memory>
#include <utility>
#include <variant>
#include <vector>
#include <fmt/format.h>

using Identifier = std::variant<int, std::string>;

class Object {
public:
    virtual ~Object() = 0;

    virtual std::string toString() = 0;

    virtual std::shared_ptr<Object> _get_item(const Identifier &identifier) = 0;

    virtual std::shared_ptr<Object> _set_item(const Identifier &identifier, std::shared_ptr<Object> &value) = 0;

    [[nodiscard]] virtual Identifier identifier() const = 0;
};

class Nil final : public Object {
public:
    ~Nil() override = default;

    std::string toString() override;

    std::shared_ptr<Object> _get_item(const Identifier &identifier) override;

    std::shared_ptr<Object> _set_item(const Identifier &identifier, std::shared_ptr<Object> &value) override;

    [[nodiscard]] Identifier identifier() const override;
};

class Number final : public Object {
public:
    int num;

    explicit Number(const int num): num(num) {
    }

    ~Number() override = default;

    std::string toString() override;

    std::shared_ptr<Object> _get_item(const Identifier &identifier) override;

    std::shared_ptr<Object> _set_item(const Identifier &identifier, std::shared_ptr<Object> &value) override;

    [[nodiscard]] Identifier identifier() const override;
};

class Array final : public Object {
public:
    std::vector<std::shared_ptr<Object> > elements;

    explicit Array(std::vector<std::shared_ptr<Object> > &elements) : elements(std::move(elements)) {
    }

    ~Array() override = default;


    std::string toString() override;

    std::shared_ptr<Object> _get_item(const Identifier &identifier) override;

    std::shared_ptr<Object> _set_item(const Identifier &identifier, std::shared_ptr<Object> &value) override;

    [[nodiscard]] Identifier identifier() const override;
};

class Map final : public Object {
public:
    std::map<std::string, std::shared_ptr<Object> > map;

    explicit Map(std::map<std::string, std::shared_ptr<Object> > elements) : map(std::move(elements)) {
    }

    ~Map() override = default;


    std::string toString() override;

    std::shared_ptr<Object> _get_item(const Identifier &identifier) override;

    std::shared_ptr<Object> _set_item(const Identifier &identifier, std::shared_ptr<Object> &value) override;

    [[nodiscard]] Identifier identifier() const override;
};

class Property final : public Object {
    std::string name;

public:
    explicit Property(std::string name) : name(std::move(name)) {
    }

    ~Property() override = default;

    std::string toString() override;

    std::shared_ptr<Object> _get_item(const Identifier &identifier) override;

    std::shared_ptr<Object> _set_item(const Identifier &identifier, std::shared_ptr<Object> &value) override;

    [[nodiscard]] Identifier identifier() const override;
};


#endif // OBJECT_H
