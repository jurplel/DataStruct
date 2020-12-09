    cmake_minimum_required(VERSION 3.5)

project(DataStruct LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dataStruct main.cpp datamodel.cpp teamoptimizer.cpp treemodel.cpp searchtree.cpp)
