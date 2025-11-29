set(LINEAR_TEST_SOURCES
    test_graph
)

foreach(test_name ${LINEAR_TEST_SOURCES})
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} mylib_graph)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()