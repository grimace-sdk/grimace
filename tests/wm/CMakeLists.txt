cmake_minimum_required(VERSION 3.20)
project(wm C)

include(Grimace)

grimace_add_executable(program program.c)
target_link_libraries(program PRIVATE grimace::aygshell)
