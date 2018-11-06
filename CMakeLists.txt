cmake_minimum_required(VERSION 3.5)

project(static_string_test)

set(CMAKE_CXX_STANDARD 14)

set(HEADERS
    static_string.h
)

set(SOURCES
    static_string_test.cpp
)

add_executable(static_string_test
    ${HEADERS}
    ${SOURCES}
)
