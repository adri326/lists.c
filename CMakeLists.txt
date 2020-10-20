cmake_minimum_required(VERSION 3.15)

project(lists-test)

add_executable(lists-test test.c)

include(include.cmake)

find_package(check REQUIRED)

target_link_libraries(lists-test PUBLIC check lists-c)
target_include_directories(lists-test PUBLIC ${LISTS_C_INCLUDE_DIR})
