//
// Created by Riebers on 03.01.2025.
//

#include "Json.hpp"

bool json::JsonContainer::is_string() const {
    return std::holds_alternative<JsonString>(m_data);
}

bool json::JsonContainer::is_number() const {
    return std::holds_alternative<JsonNumber>(m_data);
}

bool json::JsonContainer::is_boolean() const {
    return std::holds_alternative<JsonBoolean>(m_data);
}

bool json::JsonContainer::is_null() const {
    return std::holds_alternative<JsonNull>(m_data);
}

bool json::JsonContainer::is_array() const {
    return std::holds_alternative<JsonArray>(m_data);
}

bool json::JsonContainer::is_object() const {
    return std::holds_alternative<JsonObject>(m_data);
}

json::JsonString json::JsonContainer::string() const {
    return std::get<JsonString>(m_data);
}

json::JsonBoolean json::JsonContainer::boolean() const {
    return std::get<JsonBoolean>(m_data);
}

json::JsonNumber json::JsonContainer::number() const {
    return std::get<JsonNumber>(m_data);
}

json::JsonArray json::JsonContainer::array() const {
    return std::get<JsonArray>(m_data);
}

json::JsonObject json::JsonContainer::object() const {
    return std::get<JsonObject>(m_data);
}
