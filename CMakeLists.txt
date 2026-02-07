cmake_minimum_required(VERSION 3.16)
project(atomic-vec)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(atomic-vec src/main.cpp src/transaction.cpp)