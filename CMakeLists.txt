cmake_minimum_required(VERSION 3.15)
project(MarkovProject C)

set(CMAKE_C_STANDARD 11)
set(SOURCES
        main.c
        markov.c
        utils.c
        hasse.c
        # ajoute ici tous tes .c
)

add_executable(main ${SOURCES})