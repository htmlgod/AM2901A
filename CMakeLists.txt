cmake_minimum_required(VERSION 3.1)

project(am2901a)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(am2901a ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/sources/am2901a.cpp ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/include/am2901a.hpp)
target_include_directories(am2901a PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/include)
target_include_directories(am2901a PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/sources)

add_executable(am2901a_asm ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM/sources/am2901a_asm.cpp)
target_include_directories(am2901a_asm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM/include)

target_link_libraries(am2901a_asm am2901a)
