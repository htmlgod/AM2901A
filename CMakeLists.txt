cmake_minimum_required(VERSION 3.1)


project(am2901a)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/sources/test.cpp)
add_library(am2901a ${CMAKE_CURRENT_SOURCE_DIR}/sources/am2901a.cpp)
target_include_directories(am2901a PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(test am2901a)
