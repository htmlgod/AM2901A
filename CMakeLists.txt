cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(am2901a)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED False)

option(BUILD_TESTS "Build tests" OFF)

add_library(am2901a ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/sources/am2901a.cpp ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/include/am2901a.hpp)
target_include_directories(am2901a PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/include)
target_include_directories(am2901a PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A/sources)

add_library(am2901a_asm ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM/sources/am2901a_asm.cpp)
target_include_directories(am2901a_asm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM/include)
target_include_directories(am2901a_asm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM/sources)

target_link_libraries(am2901a_asm am2901a)

add_executable(asm_am2901a ${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM/sources/executable.cpp)
target_link_libraries(asm_am2901a am2901a_asm)

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(third-party/gtest)

	add_executable(cpu_test tests/am2901a/test_cpu.cpp)

	add_executable(asm_test tests/am2901a_asm/test_parser.cpp)


	target_link_libraries(asm_test am2901a_asm gtest_main)
	target_link_libraries(cpu_test am2901a gtest_main)

	add_test(NAME cpu_test COMMAND cpu_test)
	add_test(NAME asm_test COMMAND asm_test)
endif()

