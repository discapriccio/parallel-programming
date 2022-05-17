cmake_minimum_required(VERSION 3.20)
project(ASG)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
add_executable(ASG source/main.cpp source/Net.cpp head/Net.h source/Parser.cpp head/Parser.h source/Placement.cpp head/Placement.h)
