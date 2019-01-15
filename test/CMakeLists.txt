add_executable(unit_tests
    Settings.cpp
    test.mm)

set(SLANG_TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/files")
# Supply a default test root directory, so that manual testing
# doesn't have to specify the --test-root option in the normal
# case that you want to use the tests from the same source tree.
target_compile_definitions(unit_tests PRIVATE SLANG_TEST_DIRECTORY="${SLANG_TEST_DIRECTORY}")

target_link_libraries(unit_tests
    gtest_main
    slang
    objc
    "-framework Foundation"
    )

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    )