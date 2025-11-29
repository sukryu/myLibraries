cmake_minimum_required(VERSION 3.10)
project(MyLibraries VERSION 1.0)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include 디렉토리
include_directories(include)

# Libraries
add_subdirectory(src/linear)
add_subdirectory(src/tree)
add_subdirectory(src/hash)

# Tests
add_subdirectory(tests)