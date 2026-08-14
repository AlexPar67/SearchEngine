cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

configure_file(
        ${CMAKE_SOURCE_DIR}/config/config.json
        ${CMAKE_CURRENT_BINARY_DIR}/config.json
        COPYONLY
)

configure_file(
        ${CMAKE_SOURCE_DIR}/config/requests.json
        ${CMAKE_CURRENT_BINARY_DIR}/requests.json
        COPYONLY
)

configure_file(
        ${CMAKE_SOURCE_DIR}/res/file001.txt
        ${CMAKE_CURRENT_BINARY_DIR}/file001.txt
        COPYONLY
)

configure_file(
        ${CMAKE_SOURCE_DIR}/res/file002.txt
        ${CMAKE_CURRENT_BINARY_DIR}/file002.txt
        COPYONLY
)

configure_file(
        ${CMAKE_SOURCE_DIR}/res/file003.txt
        ${CMAKE_CURRENT_BINARY_DIR}/file003.txt
        COPYONLY
)

add_executable(
        SearchEngine
        main.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
)

target_include_directories(
        SearchEngine
        PUBLIC
        ${INCLUDE_DIR}
)

target_link_libraries(
        SearchEngine
        PRIVATE
        nlohmann_json::nlohmann_json
)