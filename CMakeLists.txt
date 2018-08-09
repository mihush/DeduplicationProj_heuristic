cmake_minimum_required(VERSION 3.6)
project(DeduplicationProj_heuristic)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c ObjectStructures.c TextParsingUtilities.c memory_pool.c memory_pool.h)
add_executable(DeduplicationProj_heuristic ${SOURCE_FILES} )