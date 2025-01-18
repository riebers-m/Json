//
// Created by Riebers on 17.01.2025.
//
#include <catch2/catch_all.hpp>

#include "Tokenizer.hpp"

TEST_CASE("JsonTokenizer: String") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"hello":"there"})");
    REQUIRE(tokens.value().size() == 5);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "hello");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::String);
    REQUIRE(tokens.value()[3].value == "there");
    REQUIRE(tokens.value()[4].type == json::TokenType::RightBrace);
}


TEST_CASE("JsonTokenizer: null") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"hello":null})");
    REQUIRE(tokens.value().size() == 5);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "hello");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::Null);
    REQUIRE(tokens.value()[3].value == "null");
    REQUIRE(tokens.value()[4].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: boolean1") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"hello":true})");
    REQUIRE(tokens.value().size() == 5);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "hello");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::Boolean);
    REQUIRE(tokens.value()[3].value == "true");
    REQUIRE(tokens.value()[4].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: boolean2") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"hello":false})");
    REQUIRE(tokens.value().size() == 5);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "hello");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::Boolean);
    REQUIRE(tokens.value()[3].value == "false");
    REQUIRE(tokens.value()[4].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: number1") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"hello":12345})");
    REQUIRE(tokens.value().size() == 5);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "hello");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::Number);
    REQUIRE(tokens.value()[3].value == "12345");
    REQUIRE(tokens.value()[4].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: number2") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"hello":1.2345})");
    REQUIRE(tokens.value().size() == 5);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "hello");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::Number);
    REQUIRE(tokens.value()[3].value == "1.2345");
    REQUIRE(tokens.value()[4].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: bracket") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"hello":[1,2,3]})");
    REQUIRE(tokens.value().size() == 11);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "hello");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::LeftBracket);
    REQUIRE(tokens.value()[4].type == json::TokenType::Number);
    REQUIRE(tokens.value()[5].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[6].type == json::TokenType::Number);
    REQUIRE(tokens.value()[7].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[8].type == json::TokenType::Number);
    REQUIRE(tokens.value()[9].type == json::TokenType::RightBracket);
    REQUIRE(tokens.value()[10].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: nested object") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"outer":{"inner":42}})");

    REQUIRE(tokens.value().size() == 9);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "outer");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[4].type == json::TokenType::String);
    REQUIRE(tokens.value()[4].value == "inner");
    REQUIRE(tokens.value()[5].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[6].type == json::TokenType::Number);
    REQUIRE(tokens.value()[6].value == "42");
    REQUIRE(tokens.value()[7].type == json::TokenType::RightBrace);
    REQUIRE(tokens.value()[8].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: array of strings") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"(["one","two","three"])");

    REQUIRE(tokens.value().size() == 7);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBracket);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "one");
    REQUIRE(tokens.value()[2].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[3].type == json::TokenType::String);
    REQUIRE(tokens.value()[3].value == "two");
    REQUIRE(tokens.value()[4].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[5].type == json::TokenType::String);
    REQUIRE(tokens.value()[5].value == "three");
    REQUIRE(tokens.value()[6].type == json::TokenType::RightBracket);
}

TEST_CASE("JsonTokenizer: booleans and null") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"boolTrue":true,"boolFalse":false,"nothing":null})");

    REQUIRE(tokens.value().size() == 13);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "boolTrue");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::Boolean);
    REQUIRE(tokens.value()[3].value == "true");
    REQUIRE(tokens.value()[4].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[5].type == json::TokenType::String);
    REQUIRE(tokens.value()[5].value == "boolFalse");
    REQUIRE(tokens.value()[6].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[7].type == json::TokenType::Boolean);
    REQUIRE(tokens.value()[7].value == "false");
    REQUIRE(tokens.value()[8].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[9].type == json::TokenType::String);
    REQUIRE(tokens.value()[9].value == "nothing");
    REQUIRE(tokens.value()[10].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[11].type == json::TokenType::Null);
    REQUIRE(tokens.value()[12].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: complex structure") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({
        "name": "John",
        "age": 30,
        "isStudent": false,
        "hobbies": ["reading", "coding", "hiking"],
        "address": {"city": "New York", "zip": "10001"}
    })");

    REQUIRE(tokens.value().size() == 35);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[1].value == "name");
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::String);
    REQUIRE(tokens.value()[3].value == "John");
    REQUIRE(tokens.value()[4].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[5].type == json::TokenType::String);
    REQUIRE(tokens.value()[5].value == "age");
    REQUIRE(tokens.value()[6].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[7].type == json::TokenType::Number);
    REQUIRE(tokens.value()[7].value == "30");
    REQUIRE(tokens.value()[8].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[9].type == json::TokenType::String);
    REQUIRE(tokens.value()[9].value == "isStudent");
    REQUIRE(tokens.value()[10].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[11].type == json::TokenType::Boolean);
    REQUIRE(tokens.value()[11].value == "false");
    REQUIRE(tokens.value()[12].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[13].type == json::TokenType::String);
    REQUIRE(tokens.value()[13].value == "hobbies");
    REQUIRE(tokens.value()[14].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[15].type == json::TokenType::LeftBracket);
    REQUIRE(tokens.value()[16].type == json::TokenType::String);
    REQUIRE(tokens.value()[16].value == "reading");
    REQUIRE(tokens.value()[17].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[18].type == json::TokenType::String);
    REQUIRE(tokens.value()[18].value == "coding");
    REQUIRE(tokens.value()[19].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[20].type == json::TokenType::String);
    REQUIRE(tokens.value()[20].value == "hiking");
    REQUIRE(tokens.value()[21].type == json::TokenType::RightBracket);
    REQUIRE(tokens.value()[22].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[23].type == json::TokenType::String);
    REQUIRE(tokens.value()[23].value == "address");
    REQUIRE(tokens.value()[24].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[25].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[26].type == json::TokenType::String);
    REQUIRE(tokens.value()[26].value == "city");
    REQUIRE(tokens.value()[27].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[28].type == json::TokenType::String);
    REQUIRE(tokens.value()[28].value == "New York");
    REQUIRE(tokens.value()[29].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[30].type == json::TokenType::String);
    REQUIRE(tokens.value()[30].value == "zip");
    REQUIRE(tokens.value()[31].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[32].type == json::TokenType::String);
    REQUIRE(tokens.value()[32].value == "10001");
    REQUIRE(tokens.value()[33].type == json::TokenType::RightBrace);
    REQUIRE(tokens.value()[34].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: missing closing brace") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key":"value")");

    REQUIRE(tokens.value().size() == 4);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::String);
}

TEST_CASE("JsonTokenizer: missing key-value separator") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key" "value"})");

    REQUIRE(tokens.value().size() == 4);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[2].type == json::TokenType::String);
    REQUIRE(tokens.value()[3].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: invalid character in key") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({key:"value"})");

    REQUIRE(!tokens.has_value());
    REQUIRE(tokens.error().error == json::Error::invalid_type);
    REQUIRE(tokens.error().pos == 1);
}

TEST_CASE("JsonTokenizer: unclosed string") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key":"value)");

    REQUIRE(!tokens.has_value());
    REQUIRE(tokens.error().error == json::Error::unquoted_string);
    REQUIRE(tokens.error().pos == 12);
}

TEST_CASE("JsonTokenizer: unexpected comma") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key":"value",})");

    REQUIRE(tokens.value().size() == 6);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::String);
    REQUIRE(tokens.value()[4].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[5].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: invalid number format") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key":123abc})");

    REQUIRE(!tokens.has_value());
    REQUIRE(tokens.error().error == json::Error::invalid_type);
    REQUIRE(tokens.error().pos == 10);
}

TEST_CASE("JsonTokenizer: missing closing bracket in array") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"array":[1,2,3)");

    REQUIRE(tokens.value().size() == 9);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::LeftBracket);
    REQUIRE(tokens.value()[4].type == json::TokenType::Number);
    REQUIRE(tokens.value()[5].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[6].type == json::TokenType::Number);
    REQUIRE(tokens.value()[7].type == json::TokenType::Comma);
    REQUIRE(tokens.value()[8].type == json::TokenType::Number);
}

TEST_CASE("JsonTokenizer: extra colon") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key"::"value"})");

    REQUIRE(tokens.value().size() == 6);
    REQUIRE(tokens.value()[0].type == json::TokenType::LeftBrace);
    REQUIRE(tokens.value()[1].type == json::TokenType::String);
    REQUIRE(tokens.value()[2].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[3].type == json::TokenType::KeyValueSeparator);
    REQUIRE(tokens.value()[4].type == json::TokenType::String);
    REQUIRE(tokens.value()[5].type == json::TokenType::RightBrace);
}

TEST_CASE("JsonTokenizer: invalid boolean value") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"isTrue":tru})");

    REQUIRE(!tokens.has_value());
    REQUIRE(tokens.error().error == json::Error::invalid_type);
    REQUIRE(tokens.error().pos == 10);
}

TEST_CASE("JsonTokenizer: empty input") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize("");

    REQUIRE(tokens.value().size() == 0);
}

TEST_CASE("JsonTokenizer: extra characters after JSON") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key":"value"} extra)");

    REQUIRE(!tokens.has_value());
    REQUIRE(tokens.error().error == json::Error::invalid_type);
    REQUIRE(tokens.error().pos == 16);
}

TEST_CASE("JsonTokenizer: invalid escape sequence") {
    json::detail::Tokenizer tokenizer;
    auto const tokens = tokenizer.tokenize(R"({"key":"value\q"})");

    REQUIRE(!tokens.has_value());
    REQUIRE(tokens.error().error == json::Error::invalid_escape_sequence);
    REQUIRE(tokens.error().pos == 14);
}
