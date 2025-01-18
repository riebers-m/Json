//
// Created by Riebers on 03.01.2025.
//
#include <catch2/catch_all.hpp>
#include "Json.hpp"
#include "Serialize.hpp"


using namespace json;

TEST_CASE("JsonContainer: Default Construction") {
    JsonContainer json;

    REQUIRE(json.is_null());
}

TEST_CASE("JsonContainer: String Construction and Retrieval") {
    JsonContainer json("Hello, World!");
    REQUIRE(json.is_string());

    auto result = json.string();
    REQUIRE(result.has_value());
    REQUIRE(result.value() == "Hello, World!");
}

TEST_CASE("JsonContainer: Number Construction and Retrieval") {
    JsonContainer json(42);
    REQUIRE(json.is_number());

    auto result = json.number();
    REQUIRE(result.has_value());
    REQUIRE(result.value() == 42.0); // Stored as double
}

TEST_CASE("JsonContainer: Boolean Construction and Retrieval") {
    JsonContainer json(true);
    REQUIRE(json.is_boolean());

    auto result = json.boolean();
    REQUIRE(result.has_value());
    REQUIRE(result.value() == true);
}

TEST_CASE("JsonContainer: Array Construction and Retrieval") {
    JsonArray array = {JsonContainer(1), JsonContainer(2), JsonContainer(3)};
    JsonContainer json(array);

    REQUIRE(json.is_array());

    auto result = json.array();
    REQUIRE(result.has_value());
    REQUIRE(result.value() == array); // Assumes operator== is defined for JsonArray
}

TEST_CASE("JsonContainer: Object Construction and Retrieval") {
    JsonObject object = {{JsonString{"key1"}, JsonContainer(42)}, {JsonString{"key2"}, JsonContainer("value")}};
    JsonContainer json(object);

    REQUIRE(json.is_object());

    auto result = json.object();
    REQUIRE(result.has_value());
    REQUIRE(result.value() == object); // Assumes operator== is defined for JsonObject
}

TEST_CASE("JsonContainer: Invalid Type Retrieval") {
    JsonContainer json(42); // Number

    auto string_result = json.string();
    REQUIRE_FALSE(string_result.has_value());
    REQUIRE(string_result.error() == "invalid json value access");

    auto bool_result = json.boolean();
    REQUIRE_FALSE(bool_result.has_value());
    REQUIRE(bool_result.error() == "invalid json value access");
}

TEST_CASE("JsonContainer: Null Construction and Type Check") {
    JsonContainer json(nullptr);

    REQUIRE(json.is_null());
}

//
TEST_CASE("JsonContainer: Initializer List Construction") {
    JsonContainer json({{JsonString{"key1"}, JsonContainer(123)}, {JsonString{"key2"}, JsonContainer("value")}});

    REQUIRE(json.is_object());

    auto object_result = json.object();
    REQUIRE(object_result.has_value());
    for (auto const &[key, value]: object_result.value().values) {
        if (value->is_number()) {
            REQUIRE(value->number() == 123.0);
        } else if (value->is_string()) {
            REQUIRE(value->string() == "value");
        }
    }
}

TEST_CASE("JsonContainer: Initialize std::vector") {
    std::vector<int> vector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    JsonContainer json{vector};

    REQUIRE(json.is_array());
    auto const result = json.array();
    REQUIRE(result.has_value());
    REQUIRE(result.value().values.size() == vector.size());
}

// TEST_CASE("JsonContainer: Struct to JsonContainer") {
//     struct Test {
//         int x;
//         float y;
//         double z;
//     };
//     REGISTER_MEMBER(Test, x);
//     REGISTER_MEMBER(Test, y);
//     REGISTER_MEMBER(Test, z);
//
//     Test test{.x = 1, .y = 2.0, .z = 3.0};
//     JsonContainer const json{test};
//     REQUIRE(json.is_object());
// }
