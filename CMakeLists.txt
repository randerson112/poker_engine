cmake_minimum_required(VERSION 3.10)

project(PokerEngine)

set(CMAKE_C_STANDARD 99)

add_library(poker STATIC
    src/deck.c
    src/table.c
    src/player.c
    src/hand_eval.c
    src/equity.c
)

target_include_directories(poker PUBLIC include)