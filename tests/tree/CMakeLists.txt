set(TREE_TEST_SOURCES
    test_binary_search_tree
    test_avl_tree
    test_heap
    test_hash_table
)

foreach(test_name ${TREE_TEST_SOURCES})
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} mylib_tree)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()