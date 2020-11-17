cmake_minimum_required(VERSION 3.15)

set(LISTS_C_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(LISTS_C_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/linkedlist.c")
set(LISTS_C_HEADERS "${CMAKE_CURRENT_LIST_DIR}/src/linkedlist.h" "${CMAKE_CURRENT_LIST_DIR}/src/btree.h")

add_library(lists-c ${LISTS_C_SOURCES} ${LISTS_C_HEADERS})
