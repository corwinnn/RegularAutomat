cmake_minimum_required(VERSION 3.8)
project(RegularAutomaton)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Automaton.h)
add_executable(RegularAutomaton ${SOURCE_FILES})