cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(am2901a VERSION 0.0.1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AM2901A)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AM2901A_ASM)


if (BUILD_TESTING)
	add_subdirectory(third-party/gtest)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

install(TARGETS asm_am2901a DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md DESTINATION .)

include(CPackConfig.cmake)
