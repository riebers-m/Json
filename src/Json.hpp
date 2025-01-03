//
// Created by Riebers on 03.01.2025.
//

#pragma once
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace json {
    struct JsonString {
        explicit JsonString(std::string  str) : value(std::move(str)) {};
        explicit JsonString(const char* value) : value(value) {};
        std::string value;
    };

    struct JsonNumber {
        double value;
    };

    struct JsonBoolean {
        bool value;
    };

    struct JsonNull {

    };

    class JsonContainer;

    struct JsonArray {
        JsonArray() = default;
        JsonArray(std::initializer_list<JsonContainer> init);

        std::vector<std::shared_ptr<JsonContainer>> values;
    };

    struct JsonObject {

        JsonObject(std::initializer_list<std::pair<JsonString, JsonContainer>> init);
        std::vector<std::pair<JsonString, std::shared_ptr<JsonContainer>>> values;
    };

class JsonContainer {
private:
    using JsonVariant = std::variant<JsonString, JsonNumber, JsonBoolean, JsonNull, JsonArray, JsonObject>;
    JsonVariant m_data;
    public:

    template<typename T>
    explicit JsonContainer(T const& data): JsonContainer() {
        to_json(*this, data);
    } ;

    JsonContainer() : m_data(JsonNull{}) {};

    explicit JsonContainer(int data) : m_data(JsonVariant{JsonNumber{static_cast<double>(data)}}) {}
    explicit JsonContainer(float data) : m_data(JsonVariant{JsonNumber{static_cast<double>(data)}}) {}
    explicit JsonContainer(double data) : m_data(JsonVariant{JsonNumber{data}}) {}
    explicit JsonContainer(unsigned int data) : m_data(JsonVariant{JsonNumber{static_cast<double>(data)}}) {}
    explicit JsonContainer(std::size_t data) : m_data(JsonVariant{JsonNumber{static_cast<double>(data)}}) {}

    explicit JsonContainer(std::string data) : m_data(JsonVariant{JsonString{std::move(data)}}) {}
    explicit JsonContainer(const char* data) : m_data(JsonVariant{JsonString{data}}) {}
    explicit JsonContainer(bool data) : m_data(JsonVariant{JsonBoolean{data}}) {}
    explicit JsonContainer(JsonVariant data) : m_data(std::move(data)) {}
    explicit JsonContainer(nullptr_t) : m_data(JsonNull{}) {}
    explicit JsonContainer(JsonArray data) : m_data(std::move(data)) {}
    explicit JsonContainer(JsonObject data) : m_data(std::move(data)) {}
    JsonContainer(std::initializer_list<std::pair<JsonString, JsonContainer>> init) : m_data(JsonObject{init}) {}


    explicit JsonContainer(JsonString data) : m_data(std::move(data)) {};
    explicit JsonContainer(JsonNumber data) : m_data(data) {};
    explicit JsonContainer(JsonBoolean data) : m_data(data) {};
    explicit JsonContainer(JsonNull data) : m_data(data) {};

    [[nodiscard]] bool is_string() const;
    [[nodiscard]] bool is_number() const;
    [[nodiscard]] bool is_boolean() const;
    [[nodiscard]] bool is_null() const;
    [[nodiscard]] bool is_array() const;
    [[nodiscard]] bool is_object() const;

    [[nodiscard]] std::string string() const;
    [[nodiscard]] bool boolean() const;
    [[nodiscard]] double number() const;



};

    template<typename T>
    void to_json(JsonContainer& json, T const& data) {
        json = JsonContainer{data};
    }

    template<typename T>
    void to_json(JsonContainer& json, std::vector<T> const& data) {
        JsonArray result;
        for (auto const& item : data) {
            result.values.emplace_back(std::make_shared<JsonContainer>(item));
        }
        json = JsonContainer{result};
    }
}

