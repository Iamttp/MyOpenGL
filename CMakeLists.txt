# win10
#cmake_minimum_required(VERSION 3.14)
#project(OpenGLGame)
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D FREEGLUT_STATIC")
#
#set(OPENGL_FILE F:/freeglut)
#INCLUDE_DIRECTORIES(${OPENGL_FILE}/include)
#link_directories("${OPENGL_FILE}/lib")
#
#add_executable(OpenGLGame main.cpp myDrawUtil.h)
#
#target_link_libraries(OpenGLGame freeglut_static opengl32 winmm gdi32 glu32.lib)

# ---------------------------------------------
# linux 添加pthread
cmake_minimum_required(VERSION 3.14)
project(OpenGLGame)

add_executable(OpenGLGame main.cpp myDrawUtil.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} Threads::Threads)