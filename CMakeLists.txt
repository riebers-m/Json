cmake_minimum_required(VERSION 3.30)
project(JsonParser LANGUAGES CXX)

set(JSON_VERSION_MAJOR 0)
set(JSON_VERSION_MINOR 3)
set(JSON_VERSION_PATCH 1)
set(JSON_VERSION ${JSON_VERSION_MAJOR}.${JSON_VERSION_MINOR}.${JSON_VERSION_PATCH})

include(${PROJECT_SOURCE_DIR}/project_options.cmake)
include(${PROJECT_SOURCE_DIR}/dependencies.cmake)

json_parser_setup_dependencies()

add_subdirectory(src)

if (${json_parser_build_tests})
    include(CTest)
    add_subdirectory(tests)
endif ()