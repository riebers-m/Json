add_executable(jsonTokenizer
        json_tokenizer.cpp)
target_link_libraries(jsonTokenizer PRIVATE Catch2::Catch2WithMain JsonParser::JsonParser)
set_target_properties(jsonTokenizer PROPERTIES CXX_STANDARD 20)
add_test(NAME jsonTokenizer COMMAND jsonTokenizer)

add_executable(json_serialize_deserialize
        json_serialize_deserialize.cpp
)
target_link_libraries(json_serialize_deserialize PRIVATE Catch2::Catch2WithMain JsonParser::JsonParser)
set_target_properties(json_serialize_deserialize PROPERTIES CXX_STANDARD 20)
add_test(NAME json_serialize_deserialize COMMAND json_serialize_deserialize)
