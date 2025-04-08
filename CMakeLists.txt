cmake_minimum_required(VERSION 3.30)
project(TaskTracker)

set(CMAKE_CXX_STANDARD 20)
add_executable(TaskTracker
        main.cpp
        task.cpp
        task.h
)
