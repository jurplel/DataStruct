cmake_minimum_required(VERSION 3.5)

project(dataStruct LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dataStruct main.cpp datamodel.cpp teamoptimizer.cpp treemodel.cpp)
