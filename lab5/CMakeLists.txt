cmake_minimum_required(VERSION 3.15)
project(Lab5_CustomMutex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(Lab5_CustomMutex main.cpp)