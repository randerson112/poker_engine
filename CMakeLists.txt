cmake_minimum_required(VERSION 3.10)

project(PokerEngine)

set(CMAKE_C_STANDARD 99)

add_executable(poker
    src/main.c
    src/hand_eval.c
)

target_include_directories(poker PUBLIC include)