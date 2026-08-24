cmake_minimum_required(VERSION 3.28)

project(Game2
        VERSION 0.1.0
        LANGUAGES CXX
)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
            "In-source builds are not supported. "
            "Create a separate build directory (e.g. build/) and run cmake from there.")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake)

add_subdirectory(src)
