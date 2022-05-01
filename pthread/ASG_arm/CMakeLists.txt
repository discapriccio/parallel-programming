cmake_minimum_required(VERSION 2.8)
project(ASG)

set(CMAKE_CXX_STANDARD 14)

add_executable(ASG source/main.cpp source/Net.cpp head/Net.h source/Parser.cpp head/Parser.h source/Placement.cpp head/Placement.h)
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(ASG source PUBLIC "-lpthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(ASG source "${CMAKE_THREAD_LIBES_INIT}")
endif()
