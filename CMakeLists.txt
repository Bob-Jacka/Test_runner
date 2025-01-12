cmake_minimum_required(VERSION 3.30)
project(Test_runner CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        Test_runner
        main.cpp
        Game_results.h
        custom_types.h
        Inline_funcs.h
        List.h
        Input_statements.h
)
