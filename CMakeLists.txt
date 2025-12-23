cmake_minimum_required(VERSION 3.10)

project(P2PChessGame)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/board
    ${PROJECT_SOURCE_DIR}/src/display
    ${PROJECT_SOURCE_DIR}/src/rule
)

set(MAIN_SOURCES
    src/main.c
    src/board/board.c
    src/display/display.c
    src/rule/rule.c
)

add_executable(P2PChessGame ${MAIN_SOURCES})


add_executable(test_rule test/test_rule.c src/board/board.c src/display/display.c src/rule/rule.c)
target_include_directories(test_rule PRIVATE
    ${PROJECT_SOURCE_DIR}/src/board
    ${PROJECT_SOURCE_DIR}/src/display
    ${PROJECT_SOURCE_DIR}/src/rule
)