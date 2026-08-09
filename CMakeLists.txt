cmake_minimum_required(VERSION 3.20)

project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# nlohmann/json

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(json)

# Google Test

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# Include

include_directories(include)

# Main executable

add_executable(SearchEngine
        src/main.cpp
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
)

target_link_libraries(SearchEngine
        PRIVATE
        nlohmann_json::nlohmann_json
)

# Tests

add_executable(SearchEngineTests
        tests/SearchEngineTests.cpp
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
)

target_link_libraries(SearchEngineTests
        PRIVATE
        gtest_main
        nlohmann_json::nlohmann_json
)

include(GoogleTest)

gtest_discover_tests(SearchEngineTests)