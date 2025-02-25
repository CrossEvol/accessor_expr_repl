//
// Created by mizuk on 2025/2/22.
//

#include <repl/object.h>
#include <sstream>

// Added: definition for the pure-virtual destructor.
Object::~Object() = default;

std::string Nil::toString() {
    return "Nil()";
}

std::string Number::toString() {
    return "Number(" + std::to_string(num) + ")";
}

std::string Array::toString() {
    std::stringstream ss;
    ss << "Array(";

    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << elements[i]->toString();
    }

    ss << ")";
    return ss.str();
}

std::string Map::toString() {
    std::stringstream ss;
    ss << "Map(";

    bool first = true;
    for (const auto &[key, value]: map) {
        if (!first) ss << ", ";
        ss << key << " = " << value->toString();
        first = false;
    }

    ss << ")";
    return ss.str();
}

std::shared_ptr<Object> Nil::_get_item(const Identifier &identifier) {
    throw std::logic_error(fmt::format("Not implemented _get_item on {}", typeid(*this).name()));
}

std::shared_ptr<Object> Nil::_set_item(const Identifier &identifier, std::shared_ptr<Object> &value) {
    throw std::logic_error(fmt::format("Not implemented _set_item on {}", typeid(*this).name()));
}

std::shared_ptr<Object> Number::_get_item(const Identifier &identifier) {
    throw std::logic_error(fmt::format("Not implemented _get_item on {}", typeid(*this).name()));
}

std::shared_ptr<Object> Number::_set_item(const Identifier &identifier, std::shared_ptr<Object> &value) {
    throw std::logic_error(fmt::format("Not implemented _set_item on {}", typeid(*this).name()));
}

std::shared_ptr<Object> Array::_get_item(const Identifier &identifier) {
    const auto index = std::get<int>(identifier);
    if (index < 0 || index >= elements.size()) {
        throw std::out_of_range(fmt::format("Index {} out of range", index));
    }
    return elements[index];
}

std::shared_ptr<Object> Array::_set_item(const Identifier &identifier, std::shared_ptr<Object> &value) {
    const auto index = std::get<int>(identifier);
    if (index < 0 || index >= elements.size()) {
        throw std::out_of_range(fmt::format("Index {} out of range", index));
    }
    elements[index] = value;
    return value;
}

std::shared_ptr<Object> Map::_get_item(const Identifier &identifier) {
    const auto name = std::get<std::string>(identifier);
    if (!this->map.contains(name)) {
        throw std::out_of_range(fmt::format("Key {} not found", name));
    }
    return this->map.at(name);
}

std::shared_ptr<Object> Map::_set_item(const Identifier &identifier, std::shared_ptr<Object> &value) {
    const auto name = std::get<std::string>(identifier);
    if (!this->map.contains(name)) {
        throw std::out_of_range(fmt::format("Key {} not found", name));
    }
    this->map.at(name) = value;
    return value;
}

Identifier Nil::identifier() const {
    throw std::logic_error("Not implemented");
}

Identifier Number::identifier() const {
    return this->num;
}

Identifier Array::identifier() const {
    throw std::logic_error("Not implemented");
}

Identifier Map::identifier() const {
    throw std::logic_error("Not implemented");
}

std::string Property::toString() {
    return "Property(" + this->name + ")";
}

std::shared_ptr<Object> Property::_get_item(const Identifier &identifier) {
    throw std::logic_error("Not implemented");
}

std::shared_ptr<Object> Property::_set_item(const Identifier &identifier, std::shared_ptr<Object> &value) {
    throw std::logic_error("Not implemented");
}

Identifier Property::identifier() const {
    return this->name;
}
