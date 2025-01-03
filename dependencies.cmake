include("${PROJECT_SOURCE_DIR}/cmake/CPM.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/system_link.cmake")

function(json_parser_setup_dependencies)
    # Add expected till we use c++23
    CPMAddPackage(
            NAME expected
            GITHUB_REPOSITORY TartanLlama/expected
            VERSION 1.1.0
            OPTIONS "EXPECTED_BUILD_TESTS OFF"
    )

    if (${json_parser_build_tests})
        CPMAddPackage(
                NAME Catch2
                GITHUB_REPOSITORY catchorg/Catch2
                VERSION 3.7.1
                OPTIONS "CATCH_INSTALL_HELPERS OFF" "CATCH_BUILD_TESTING OFF" "CATCH_INSTALL_DOCS OFF"
        )
    endif ()
endfunction()