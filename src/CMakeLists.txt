cmake_minimum_required(VERSION 2.8)
project(LakeConcus)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -lstdc++ --std=c++11 )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)

# add the binary tree to the search path for include files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(Fifos)
add_subdirectory(Handlers)
add_subdirectory(Lock)
add_subdirectory(Logger)
add_subdirectory(Pipes)
add_subdirectory(Semaphore)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h)

# add the executable
add_executable(LakeConcu LakeConcu.cpp ${SRCS} ${HDRS})
target_link_libraries(LakeConcu Lock Fifos Handlers Lock Logger Pipes Semaphore)
