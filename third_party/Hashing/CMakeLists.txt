cmake_minimum_required(VERSION 3.16)
project(xxhash C)

add_library(xxhash STATIC
    xxHash/xxhash.c
    xxHash/xxh_x86dispatch.c
)
target_include_directories(xxhash PUBLIC xxHash)
