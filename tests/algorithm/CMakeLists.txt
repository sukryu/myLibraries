# Algorithm tests (header-only library, no linking needed)
set(ALGORITHM_TEST_SOURCES
    test_sorting
)

foreach(test_name ${ALGORITHM_TEST_SOURCES})
    add_executable(${test_name} ${test_name}.cpp)
    # Header-only, no library to link
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()