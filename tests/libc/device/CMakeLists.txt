cmake_minimum_required(VERSION 3.20)
project(libc C)

include(Grimace)

grimace_add_executable(libc libc.c ICON libc.ico)
target_link_libraries(libc PRIVATE grimace::posix)
