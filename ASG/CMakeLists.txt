cmake_minimum_required(VERSION 3.20)
project(ASG)

set(CMAKE_CXX_STANDARD 14)

add_executable(ASG source/main.cpp source/Net.cpp head/Net.h source/Parser.cpp head/Parser.h source/Placement.cpp head/Placement.h)
