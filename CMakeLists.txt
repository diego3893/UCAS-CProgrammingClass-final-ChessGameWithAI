cmake_minimum_required(VERSION 3.10)

project(P2PChessGame)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/board
    ${PROJECT_SOURCE_DIR}/src/display
    ${PROJECT_SOURCE_DIR}/src/rule
    ${PROJECT_SOURCE_DIR}/src/ai
)

set(MAIN_SOURCES
    src/main.c
    src/board/board.c
    src/display/display.c
    src/rule/rule.c
    src/ai/ai.c
    src/ai/evaluate.c
    src/ai/tt.c
)

add_executable(ChessGame ${MAIN_SOURCES})


add_executable(test_rule test/test_rule.c src/board/board.c src/display/display.c src/rule/rule.c)
target_include_directories(test_rule PRIVATE
    ${PROJECT_SOURCE_DIR}/src/board
    ${PROJECT_SOURCE_DIR}/src/display
    ${PROJECT_SOURCE_DIR}/src/rule
)