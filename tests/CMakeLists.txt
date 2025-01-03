add_executable(jsonParserTests json_parser_tests.cpp)
#target_include_directories(jsonParserTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(jsonParserTests Catch2::Catch2WithMain)
add_test(NAME jsonParserTests COMMAND jsonParserTests)
