cmake_minimum_required(VERSION 2.0)
project(projectP)

add_subdirectory(libs)

set(EXECUTABLE_NAME "punkt")
add_executable(${EXECUTABLE_NAME} main.c)

target_link_libraries (${EXECUTABLE_NAME} libs) 
