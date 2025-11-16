cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include 디렉토리
include_directories(include)

#Add Library
add_library(mylib STATIC
    src/linear/dynamic_array.cpp)

# Test executor
add_executable(test_dynamic_array
    tests/test_dynamic_array.cpp)

target_link_libraries(test_dynamic_array mylib)

# Enable tests
enable_testing()
add_test(NAME DynamicArrayTest COMMAND test_dynamic_array)

# 메인 실행 파일
#add_executable(main src/main.cpp)

# 추가 소스 파일이 있다면 여기에 추가
# add_executable(main src/main.cpp src/other.cpp)
