
cmake_minimum_required(VERSION 2.0)
project(projectP)

find_package(GLUT)
find_package(OpenGL)

add_subdirectory(libs)
add_subdirectory(shaders)
add_subdirectory(res)

set(EXECUTABLE_NAME "punkt")
add_executable(${EXECUTABLE_NAME} main.c terrain.c objects.c camera.c)


IF(UNIX)
  find_library(M_LIB m)
  target_link_libraries(${EXECUTABLE_NAME} ${M_LIB})
ENDIF(UNIX)

target_link_libraries (${EXECUTABLE_NAME} libs ${GLUT_LIBRARY} ${GL_LIBRARY})
