cmake_minimum_required(VERSION 3.20)

project(SearchEngineTests)

include(FetchContent)

# GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        SearchEngineTests
        SearchEngineTests.cpp
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
)

target_include_directories(
        SearchEngineTests
        PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(
        SearchEngineTests
        PRIVATE
        gtest_main
)

include(GoogleTest)

gtest_discover_tests(SearchEngineTests)