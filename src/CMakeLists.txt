message(STATUS "Building ${PROJECT_NAME}@${JSON_VERSION} (v${JSON_VERSION})")
add_library(JsonParser STATIC
        json/MemberId.hpp
        json/Json.hpp
        json/SerializerConcept.hpp
        json/Tokenizer.hpp
        json/Tokenizer.cpp
        json/TypeHash.hpp
        json/TypeId.hpp
        json/TypeInfo.hpp
        json/VariableId.hpp
)

target_link_libraries(JsonParser PUBLIC tl::expected PRIVATE json_parser_options)
target_include_directories(JsonParser PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include}>
)
set_target_properties(JsonParser PROPERTIES
        CXX_STANDARD 20
        VERSION ${JSON_VERSION}
        SOVERSION ${JSON_VERSION_MAJOR}
)

add_library(JsonParser::JsonParser ALIAS JsonParser)