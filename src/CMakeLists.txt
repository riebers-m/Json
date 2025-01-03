add_library(JsonParser STATIC library.cpp
        Json.cpp
        Json.hpp)

target_link_libraries(JsonParser PRIVATE tl::expected)
target_include_directories(JsonParser PRIVATE ${CMAKE_SOURCE_DIR}/src)