add_executable(asm_test ${CMAKE_CURRENT_SOURCE_DIR}/am2901a_asm/test_asm.cpp)

target_link_libraries(asm_test am2901a_asm gtest_main)

add_test(NAME asm_test COMMAND asm_test)