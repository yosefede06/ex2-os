cmake_minimum_required(VERSION 3.21)
project(ex2_os)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(ex2_os
        demo_itimer.c
        demo_jmp.c
        demo_singInt_handler.c
        uthreads.cpp
        uthreads.h Scheduler.h Scheduler.cpp)
