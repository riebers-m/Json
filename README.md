# Json Library for C++

This library allows you to easily serialize and deserialize complex objects, including nested and container-based types,
with minimal boilerplate.

## Features

- **Automatic Serialization/Deserialization**: Simplify JSON handling for custom data structures.
- **Type Registration**: Register members of a struct/class using `REGISTER_MEMBER`.
- **Nested Object Support**: Handle nested objects effortlessly.
- **Support for STL Containers**: Works with standard containers like `std::vector`, `std::optional`
- **Type Safety**: Ensures data validity during deserialization.

---

## TODO

1. Make library none recursive
2. Add stl container support:
    1. std::map
    2. std::set
    3. std::list
    4. std::array

---

## Installation

1. Clone the repo

```bash
git clone https://github.com/riebers-m/Json.git
```

2. Link against library

````cmake
target_link_libraries(${PROJECT_NAME} PRIVATE JsonParser::JsonParser)
````

## Usage

### Define Your Data Structures

Define your custom types and use REGISTER_MEMBER to register their members for serialization and deserialization.

````c++
#include "Json.h"

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
````

### Serialize Data to JSON

Create an instance of your data structure and serialize it into JSON format.

````c++
Foo foo{
    30, 
    "John Doe", 
    {12345, "New York", "5th Avenue", {"item1", "item2"}}
};

std::string json = json::serialize_type(foo);
std::cout << json;
````

Output

````c++
{"age": 30,"name": "John Doe","bar": {"zip": 12345,"town": "New York","street": "5th Avenue","list": ["item1", "item2"]} }
````

### Deserialize JSON to Data

Parse a JSON string into your custom data structure.

````c++
std::string json = R"({
    "age": 25,
    "name": "Jane Smith",
    "bar": {
        "zip": 54321,
        "town": "Los Angeles",
        "street": "Main Street",
        "list": ["itemA", "itemB"]
    }
})";

Foo foo;
auto result = json::deserialize_type(json, foo);

if (result.error == json::Error::ok) {
    std::cout << "Deserialization successful!" << std::endl;
} else {
    std::cerr << "Error during deserialization: " << result.error << std::endl;
}
````

### Skip Unknown Members

If the JSON contains fields not present in your struct, the library will skip them gracefully.

## Requirements

* C++ 20 or later.

## Contribution

Type reflection is base on the excellent tutorial of Connordemeyer: https://www.youtube.com/watch?v=qQ6rR5BRipE