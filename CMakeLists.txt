cmake_minimum_required(VERSION 3.17)
project(RayTracingInOneWeekend)

set(CMAKE_CXX_STANDARD 14)

# if using gcc
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
# if using MSVC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")


add_executable(PathTracer main.cpp)