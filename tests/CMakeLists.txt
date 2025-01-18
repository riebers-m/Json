#add_executable(jsonParserTests json_parser_tests.cpp)
#target_include_directories(jsonParserTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
#target_link_libraries(jsonParserTests PRIVATE Catch2::Catch2WithMain JsonParser::JsonParser)
#set_target_properties(jsonParserTests PROPERTIES
#        CXX_STANDARD 20
#)
#
#add_test(NAME jsonParserTests COMMAND jsonParserTests)

add_executable(jsonTokenizer
        json_tokenizer.cpp ${CMAKE_SOURCE_DIR}/src/examples/Tokenizer.cpp)
target_include_directories(jsonTokenizer PRIVATE ${CMAKE_SOURCE_DIR}/src/examples)
target_link_libraries(jsonTokenizer PRIVATE Catch2::Catch2WithMain tl::expected)
set_target_properties(jsonTokenizer PROPERTIES CXX_STANDARD 20)
add_test(NAME jsonTokenizer COMMAND jsonTokenizer)