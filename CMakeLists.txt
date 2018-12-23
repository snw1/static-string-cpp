cmake_minimum_required(VERSION 3.5)

project(static_string_test)

set(CMAKE_CXX_STANDARD 14)

add_executable(static_string_test
    static_string.h
    static_string_test.cpp
)
