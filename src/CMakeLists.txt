message(STATUS "Building ${PROJECT_NAME}@${JSON_VERSION} (v${JSON_VERSION})")
add_library(JsonParser STATIC
        Json.cpp
        Json.hpp
        Serialize.hpp)

target_link_libraries(JsonParser PUBLIC tl::expected json_parser_options)
target_include_directories(JsonParser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include}>
)
set_target_properties(JsonParser PROPERTIES
        CXX_STANDARD 20
        VERSION ${JSON_VERSION}
        SOVERSION ${JSON_VERSION_MAJOR}
)

add_library(JsonParser::JsonParser ALIAS JsonParser)

add_subdirectory(examples)