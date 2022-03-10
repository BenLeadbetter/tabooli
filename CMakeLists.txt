cmake_minimum_required(VERSION 3.21.0)
project(tabooli)

option(TABOOLI_TESTS "build test targets" ${PROJECT_IS_TOP_LEVEL})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(tabooli)

add_library(tabooli INTERFACE)
target_include_directories(tabooli INTERFACE inc)
tabooli_set_target_properties(tabooli)

if (TABOOLI_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()