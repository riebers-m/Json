add_executable(jsonParserTests json_parser_tests.cpp)
target_include_directories(jsonParserTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(jsonParserTests PRIVATE Catch2::Catch2WithMain JsonParser::JsonParser)
set_target_properties(jsonParserTests PROPERTIES
        CXX_STANDARD 20
)

add_test(NAME jsonParserTests COMMAND jsonParserTests)
