cmake_minimum_required(VERSION 3.17)
project(syntactic_analysis)

set(CMAKE_CXX_STANDARD 17)

add_executable(syntactic_analysis main.cpp header/utils.h source/syntactic_analyzer.cpp header/syntactic_analyzer.h)