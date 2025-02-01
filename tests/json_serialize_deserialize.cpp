//
// Created by Riebers on 19.01.2025.
//
#include <catch2/catch_all.hpp>
#include <optional>
#include <vector>
#include "json/Json.hpp"

struct File {
    std::filesystem::path path;
};

REGISTER_MEMBER(File, path);

struct Bar {
    int zip{};
    std::string town{};
    std::string street{};
    std::vector<std::string> list{};
};
REGISTER_MEMBER(Bar, zip);
REGISTER_MEMBER(Bar, town);
REGISTER_MEMBER(Bar, street);
REGISTER_MEMBER(Bar, list);

struct Foo {
    int age{};
    std::string name{};
    Bar bar;
};
REGISTER_MEMBER(Foo, age);
REGISTER_MEMBER(Foo, name);
REGISTER_MEMBER(Foo, bar);


struct Test {
    int x{};
    float y{};
    double z{};
    std::optional<int> a{};
    Foo foo;
    std::vector<Bar> bars;
};

REGISTER_MEMBER(Test, x);
REGISTER_MEMBER(Test, y);
REGISTER_MEMBER(Test, z);
REGISTER_MEMBER(Test, a);
REGISTER_MEMBER(Test, foo);
REGISTER_MEMBER(Test, bars);

TEST_CASE("Deserialize simple") {
    Foo foo;

    auto const result = json::deserialize_type(
            R"({"age":25, "name":"John Doe", "bar":{"zip":27321, "town": "A town", "street": "A street", "list":["apple", "banana"]}})",
            foo);
    REQUIRE(result.error == json::Error::ok);
    REQUIRE(foo.age == 25);
    REQUIRE(foo.name == "John Doe");
    REQUIRE(foo.bar.zip == 27321);
    REQUIRE(foo.bar.town == "A town");
    REQUIRE(foo.bar.street == "A street");
    REQUIRE(foo.bar.list[0] == "apple");
    REQUIRE(foo.bar.list[1] == "banana");
}

struct Config {
    bool boolean{};
    std::optional<bool> opt{};
};

REGISTER_MEMBER(Config, boolean);
REGISTER_MEMBER(Config, opt);

TEST_CASE("Deserialize bool") {
    SECTION("All true") {
        Config config;
        json::deserialize_type(R"({"boolean":true, "opt":true})", config);
        REQUIRE(config.boolean == true);
        REQUIRE(config.opt.value() == true);
    }

    SECTION("Null value") {
        Config config{true, true};
        REQUIRE(config.boolean);
        REQUIRE(config.opt.value());

        json::deserialize_type(R"({"boolean":false, "opt":null})", config);
        REQUIRE(config.boolean == false);
        REQUIRE(!config.opt.has_value());
    }
}

TEST_CASE("Deserialize std::filesystem::path") {
    File file;

    SECTION("empty") {
        json::deserialize_type(R"()", file);
        REQUIRE(file.path.empty());
    }
    SECTION("empty path") {
        json::deserialize_type(R"({"path":""})", file);
        REQUIRE(file.path.empty());
    }
    SECTION("path absolute") {
        json::deserialize_type(R"({"path":"/home/user/test"})", file);
        REQUIRE(file.path == std::filesystem::path{"/home/user/test"});
    }
    SECTION("path relative") {
        json::deserialize_type(R"({"path":"relative/user/test"})", file);
        REQUIRE(file.path == std::filesystem::path{"relative/user/test"});
    }
    SECTION("path windows") {
        json::deserialize_type(R"({"path":"relative\user\test"})", file);
        REQUIRE(file.path.empty());
    }
}
