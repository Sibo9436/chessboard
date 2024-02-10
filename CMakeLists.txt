cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True) 

project(chess VERSION 0.1)

add_subdirectory("vendor")
add_subdirectory("src")
add_subdirectory("app")
