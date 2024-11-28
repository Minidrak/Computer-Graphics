cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Lab_02)

# Установка версии C++
set(CMAKE_CXX_STANDARD 17)

# Укажите путь к SFML
set(SFML_DIR "C:/C++_libs/SFML-2.6.1/lib/cmake/SFML")

# Укажите пути к GLFW (замените на ваши пути)
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")  
set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/Lab_02/lib/libglfw3.a") 

# Путь к GLAD
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Lab_02/include")  
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/Lab_02/src/GLAD.c")       

# Добавляем исходные файлы
add_executable(Lab_02 src/main.cpp ${GLAD_SRC})

# Указываем путь к библиотекам SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Подключаем SFML к проекту
target_link_libraries(Lab_02 sfml-graphics sfml-window sfml-system opengl32)

# Подключаем библиотеки OpenGL
find_package(OpenGL REQUIRED)

# Указываем директории для GLFW и GLAD
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

# Подключаем библиотеки GLFW и OpenGL
# Подключаем библиотеки GLFW, OpenGL и GLU
target_link_libraries(Lab_02 ${GLFW_LIBRARY} ${OPENGL_gl_LIBRARY} glu32)

find_package(GLUT REQUIRED)

