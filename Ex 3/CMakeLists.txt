cmake_minimum_required(VERSION 3.12)
project(Ex_3 C)

set("${CMAKE_C_FLAGS} -std=c99 -Wall pendatic")

add_executable(Ex_3 infi.c infi.h NumericalAnalyzer.c )
target_link_libraries(Ex_3 m)