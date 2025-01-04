//
// Created by Riebers on 03.01.2025.
//
#include <catch2/catch_all.hpp>
#include "Json.hpp"

TEST_CASE("JsonContainer", "[JsonParser]") {
    SECTION("Default") {
        json::JsonContainer container;
        REQUIRE(container.is_null());
    }
    SECTION("Number") {
        json::JsonContainer container{json::JsonNumber{1234}};
        REQUIRE(container.is_number());
        REQUIRE(container.number() == json::JsonNumber{1234});
    }
}