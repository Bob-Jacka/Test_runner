cmake_minimum_required(VERSION 3.30)
project(Test_runner C)

set(CMAKE_C_STANDARD 11)

add_executable(Test_runner main.c
        prototypes.h game_results.h)
