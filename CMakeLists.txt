cmake_minimum_required(VERSION 3.30)
project(ring_buffer C)

set(CMAKE_C_STANDARD 99)

add_executable(ring_buffer main.c ringbuffer.c)
