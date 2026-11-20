cmake_minimum_required(VERSION 3.16)
project(PolygonCheckerTests LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# C source files without main.c
add_library(polygon_core STATIC
    triangleSolver.c
    four_points.c
)

target_include_directories(polygon_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(polygon_core PRIVATE /W4)
else()
    target_compile_options(polygon_core PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

# GoogleTest executable
add_executable(polygon_checker_tests
    tests/test_polygon_checker.cpp
)

target_link_libraries(polygon_checker_tests PRIVATE
    polygon_core
    GTest::gtest_main
)

if(UNIX AND NOT APPLE)
    target_link_libraries(polygon_checker_tests PRIVATE m)
endif()

gtest_discover_tests(polygon_checker_tests)