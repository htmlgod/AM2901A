add_library(am2901a ${CMAKE_CURRENT_SOURCE_DIR}/sources/am2901a.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/am2901a.hpp)
target_include_directories(am2901a PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(am2901a PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
