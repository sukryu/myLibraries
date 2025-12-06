# benchmarks/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Include benchmark utilities
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)

# Threads library (required for benchmarks)
find_package(Threads REQUIRED)

# ============================================
# Utility Test
# ============================================

add_executable(test_benchmark_utils
    test_benchmark_utils.cpp
)

target_link_libraries(test_benchmark_utils
    Threads::Threads
)

# ============================================
# Tree Benchmarks
# ============================================

# Balanced Tree Benchmark (AVL vs RB vs SkipList)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tree/balanced_tree_benchmark.cpp)
    add_executable(benchmark_balanced_tree
        tree/balanced_tree_benchmark.cpp
    )
    
    target_link_libraries(benchmark_balanced_tree
        mylib_tree           # Tree library
        mylib_hash           # Hash library (if needed)
        mylib_graph          # Graph library (if needed)
        Threads::Threads
    )
    
    message(STATUS "Added benchmark: balanced_tree")
endif()

# ============================================
# Algorithm Benchmarks
# ============================================

# Sorting Benchmark
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/algorithm/sorting_benchmark.cpp)
    add_executable(benchmark_sorting
        algorithm/sorting_benchmark.cpp
    )
    
    target_link_libraries(benchmark_sorting
        mylib_tree
        mylib_hash
        mylib_graph
        Threads::Threads
    )
    
    message(STATUS "Added benchmark: sorting")
endif()

# Range Query Benchmark (Segment vs Fenwick)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tree/range_query_benchmark.cpp)
    add_executable(benchmark_range_query
        tree/range_query_benchmark.cpp
    )
    
    target_link_libraries(benchmark_range_query
        mylib_tree
        mylib_hash
        mylib_graph
        Threads::Threads
    )
    
    message(STATUS "Added benchmark: range_query")
endif()

# ============================================
# Install (optional)
# ============================================

# Install benchmark executables to bin/benchmarks
install(TARGETS test_benchmark_utils
    RUNTIME DESTINATION bin/benchmarks
    COMPONENT benchmarks
)

if(TARGET benchmark_balanced_tree)
    install(TARGETS benchmark_balanced_tree
        RUNTIME DESTINATION bin/benchmarks
        COMPONENT benchmarks
    )
endif()

if(TARGET benchmark_sorting)
    install(TARGETS benchmark_sorting
        RUNTIME DESTINATION bin/benchmarks
        COMPONENT benchmarks
    )
endif()

if(TARGET benchmark_range_query)
    install(TARGETS benchmark_range_query
        RUNTIME DESTINATION bin/benchmarks
        COMPONENT benchmarks
    )
endif()

# ============================================
# Custom targets for running benchmarks
# ============================================

# Run all benchmarks
add_custom_target(run_all_benchmarks
    COMMENT "Running all benchmarks..."
)

if(TARGET benchmark_balanced_tree)
    add_custom_target(run_benchmark_balanced_tree
        COMMAND benchmark_balanced_tree
        DEPENDS benchmark_balanced_tree
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running balanced tree benchmark..."
    )
    add_dependencies(run_all_benchmarks run_benchmark_balanced_tree)
endif()

if(TARGET benchmark_sorting)
    add_custom_target(run_benchmark_sorting
        COMMAND benchmark_sorting
        DEPENDS benchmark_sorting
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running sorting benchmark..."
    )
    add_dependencies(run_all_benchmarks run_benchmark_sorting)
endif()

# ============================================
# Summary
# ============================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "  Benchmark Configuration")
message(STATUS "========================================")
message(STATUS "  Utility test: test_benchmark_utils")
message(STATUS "  Output: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "========================================")
message(STATUS "")