//
// Created by Riebers on 03.01.2025.
//

#include "Json.hpp"
#include <algorithm>

bool json::JsonString::operator==(const std::string &other) const { return value == other; }

json::JsonArray::JsonArray(std::initializer_list<JsonContainer> init) {
    for (const auto &it: init) {
        values.emplace_back(std::make_shared<JsonContainer>(it));
    }
}

bool json::JsonArray::operator==(const JsonArray &rhs) const { return std::ranges::equal(values, rhs.values); }

json::JsonObject::JsonObject(std::initializer_list<std::pair<JsonString, JsonContainer>> init) {
    for (const auto &it: init) {
        values.emplace_back(it.first, std::make_shared<JsonContainer>(it.second));
    }
}

bool json::JsonObject::operator==(const JsonObject &rhs) const { return std::ranges::equal(values, rhs.values); }

bool json::JsonContainer::is_string() const { return is<JsonString>(); }

bool json::JsonContainer::is_number() const { return is<JsonNumber>(); }

bool json::JsonContainer::is_boolean() const { return is<JsonBoolean>(); }

bool json::JsonContainer::is_null() const { return is<JsonNull>(); }

bool json::JsonContainer::is_array() const { return is<JsonArray>(); }

bool json::JsonContainer::is_object() const { return is<JsonObject>(); }

tl::expected<std::string, std::string> json::JsonContainer::string() const {
    return retrieve_as<JsonString>().map([](JsonString &&json_string) { return json_string.value; });
}

tl::expected<bool, std::string> json::JsonContainer::boolean() const {
    return retrieve_as<JsonBoolean>().map([](JsonBoolean &&json_boolean) { return json_boolean.value; });
}

tl::expected<double, std::string> json::JsonContainer::number() const {
    return retrieve_as<JsonNumber>().map([](JsonNumber &&json_number) { return json_number.value; });
}

tl::expected<json::JsonArray, std::string> json::JsonContainer::array() const { return retrieve_as<JsonArray>(); }

tl::expected<json::JsonObject, std::string> json::JsonContainer::object() const {
    return retrieve_as<JsonObject>();
}
