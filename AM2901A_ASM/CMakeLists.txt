add_library(am2901a_asm ${CMAKE_CURRENT_SOURCE_DIR}/sources/am2901a_asm.cpp)
add_executable(asm_am2901a ${CMAKE_CURRENT_SOURCE_DIR}/sources/executable.cpp)

target_include_directories(am2901a_asm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(am2901a_asm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sources)

target_link_libraries(am2901a_asm am2901a)
target_link_libraries(asm_am2901a am2901a_asm)
