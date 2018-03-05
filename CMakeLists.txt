cmake_minimum_required(VERSION 3.6)
project(DeduplicationProj_heuristic)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c List.c ObjectStructures.c HashTable.c)
add_executable(DeduplicationProj_heuristic ${SOURCE_FILES} HashTable.h)