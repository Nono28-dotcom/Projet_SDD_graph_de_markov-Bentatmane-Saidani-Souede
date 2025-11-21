cmake_minimum_required(VERSION 3.15)
project(MarkovProject C)

set(CMAKE_C_STANDARD 11)
set(SOURCES)

add_executable(main
        main.c
        markov.c
        utils.c
        hasse.c
        markov.h
        utils.h
        hasse.h
        matrix.c)