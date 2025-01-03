cmake_minimum_required(VERSION 3.30)
project(JsonParser)

set(CMAKE_MAJOR_VERSION 0)
set(CMAKE_MINOR_VERSION 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CSS_STANDARD_REQUIRED ON)

include(${PROJECT_SOURCE_DIR}/project_options.cmake)
include(${PROJECT_SOURCE_DIR}/dependencies.cmake)

json_parser_setup_dependencies()

add_subdirectory(src)

if(${json_parser_build_tests})
    include(CTest)
    add_subdirectory(tests)
endif ()