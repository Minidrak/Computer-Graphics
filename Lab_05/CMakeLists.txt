cmake_minimum_required(VERSION 3.10)

project(Lab_05)

set(CMAKE_CXX_STANDARD 17)

# Пути для библиотек
set(SFML_DIR "C:/C++_libs/SFML-2.6.1/lib/cmake/SFML")
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(GLM_INCLUDE_DIR "C:/C++_libs/glm")
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Lab_05/include")

set(GLAD_SRC "${CMAKE_SOURCE_DIR}/Lab_05/src/GLAD.c")



set(SOURCES
    ${CMAKE_SOURCE_DIR}/Lab_05/src/main.cpp
    ${CMAKE_SOURCE_DIR}/Lab_05/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/Lab_05/src/Sphere.cpp
    ${CMAKE_SOURCE_DIR}/Lab_05/src/Plane.cpp
    ${GLAD_SRC}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/Lab_05/include
    ${GLAD_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

add_executable(Lab_05 ${SOURCES})

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(Lab_05 sfml-graphics sfml-window sfml-system opengl32)

find_package(OpenGL REQUIRED)
target_link_libraries(Lab_05 ${OPENGL_gl_LIBRARY} glu32)

find_package(GLUT REQUIRED)