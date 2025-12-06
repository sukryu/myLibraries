cmake_minimum_required(VERSION 3.10)
project(MyLibraries VERSION 1.0)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include 디렉토리
include_directories(include)

# Libraries
add_subdirectory(src/linear)
add_subdirectory(src/tree)
add_subdirectory(src/hash)
add_subdirectory(src/graph)

# Tests
add_subdirectory(tests)
add_subdirectory(benchmarks)

# Print configuration summary
message(STATUS "")
message(STATUS "========================================")
message(STATUS "  myLibraries Configuration Summary")
message(STATUS "========================================")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "========================================")
message(STATUS "")