add_executable(cpu_test ${CMAKE_CURRENT_SOURCE_DIR}/am2901a/test_cpu.cpp)
add_executable(asm_test ${CMAKE_CURRENT_SOURCE_DIR}/am2901a_asm/test_parser.cpp)

target_link_libraries(cpu_test am2901a gtest_main)
target_link_libraries(asm_test am2901a_asm gtest_main)

add_test(NAME cpu_test COMMAND cpu_test)
add_test(NAME asm_test COMMAND asm_test)