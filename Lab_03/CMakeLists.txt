cmake_minimum_required(VERSION 3.10)

project(Lab_03)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/C++_libs/SFML-2.6.1/lib/cmake/SFML")


set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")  
set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/Lab_02/lib/libglfw3.a")  

set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Lab_02/include")  
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/Lab_02/src/GLAD.c")       

add_executable(Lab_03 src/main.cpp ${GLAD_SRC})

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(Lab_03 sfml-graphics sfml-window sfml-system opengl32)

find_package(OpenGL REQUIRED)

include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

target_link_libraries(Lab_03 ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY} glu32)

find_package(GLUT REQUIRED)

