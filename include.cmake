cmake_minimum_required(VERSION 3.15)

set(LISTS_C_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(LISTS_C_SOURCES src/linkedlist.c)
set(LISTS_C_HEADERS src/linkedlist.h)

add_library(lists-c ${LISTS_C_SOURCES} ${LISTS_C_HEADERS})
