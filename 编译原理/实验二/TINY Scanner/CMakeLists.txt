cmake_minimum_required(VERSION 3.10.1)
project(TINY)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(TINY src/main.c src/scan.c src/util.c)