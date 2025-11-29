set(LINEAR_TEST_SOURCES
    test_dynamic_array
    test_stack
    test_linked_list
    test_queue
    test_deque
)

foreach(test_name ${LINEAR_TEST_SOURCES})
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} mylib_linear)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()