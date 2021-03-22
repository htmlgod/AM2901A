cmake_minimum_required(VERSION 3.1)


project(am2901a)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(am2901a INTERFACE)
target_include_directories(am2901a INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/sources/test.cpp)
target_link_libraries(test am2901a)
