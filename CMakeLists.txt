cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(am2901a VERSION 1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AM2901A)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM)

add_subdirectory(third-party/gtest)

if (BUILD_TESTING)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
