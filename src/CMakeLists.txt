cmake_minimum_required(VERSION 2.8)
project(LakeConcus)

add_compile_options(-Wall -Wextra -Wpedantic)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)

# add the binary tree to the search path for include files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(Fifos)
add_subdirectory(Handlers)
add_subdirectory(Logger)
add_subdirectory(Pipes)

# add the executable
add_executable(LakeConcu LakeConcu.cpp)
target_link_libraries(LakeConcu Fifos Logger Handlers Pipes)

