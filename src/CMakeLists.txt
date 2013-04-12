cmake_minimum_required(VERSION 2.0)
project(projectP)

#find_package(GLUT REQUIRED)
#find_package(OpenGL REQUIRED)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


add_subdirectory(libs)
add_subdirectory(shaders)
add_subdirectory(res)

set(EXECUTABLE_NAME "punkt")
add_executable(${EXECUTABLE_NAME} main.c terrain.c objects.c camera.c)


IF(UNIX)
  find_library(M_LIB m)
  target_link_libraries(${EXECUTABLE_NAME} ${M_LIB})
ENDIF(UNIX)

#target_link_libraries (${EXECUTABLE_NAME} ${GL_LIBRARY} ${GLUT_LIBRARY} libs)
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} libs)

