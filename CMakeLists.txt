cmake_minimum_required(VERSION 3.13)
project(OctQuant)

set(CMAKE_CXX_STANDARD 14)

include_directories(${OctQuant_SOURCE_DIR}/include)
add_subdirectory(src)
add_subdirectory(test)
