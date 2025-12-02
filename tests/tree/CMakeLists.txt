set(TREE_TEST_SOURCES
    test_binary_search_tree
    test_avl_tree
    test_heap
)

foreach(test_name ${TREE_TEST_SOURCES})
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} mylib_tree)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Header-only tree structures (no library linking needed)
add_executable(test_trie test_trie.cpp)
add_test(NAME test_trie COMMAND test_trie)

add_executable(test_red_black_tree test_red_black_tree.cpp)
add_test(NAME test_red_black_tree COMMAND test_red_black_tree)

add_executable(test_b_tree test_b_tree.cpp)
add_test(NAME test_b_tree COMMAND test_b_tree)

add_executable(test_segment_tree test_segment_tree.cpp)
add_test(NAME test_segment_tree COMMAND test_segment_tree)

add_executable(test_fenwick_tree test_fenwick_tree.cpp)
add_test(NAME test_fenwick_tree COMMAND test_fenwick_tree)

add_executable(test_skip_list test_skip_list.cpp)
add_test(NAME test_skip_list COMMAND test_skip_list)