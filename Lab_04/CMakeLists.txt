cmake_minimum_required(VERSION 3.10)

project(Lab_04)

set(CMAKE_CXX_STANDARD 17)

# Пути для библиотек
set(SFML_DIR "C:/C++_libs/SFML-2.6.1/lib/cmake/SFML")
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Lab_04/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/Lab_04/src/GLAD.c")
set(GLM_INCLUDE_DIR "C:/C++_libs/glm")  # Добавляем путь к GLM

# Создание исполняемого файла
add_executable(Lab_04 src/main.cpp ${GLAD_SRC})

# Подключение SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(Lab_04 sfml-graphics sfml-window sfml-system opengl32)

# Подключение OpenGL и GLAD
find_package(OpenGL REQUIRED)
include_directories($ ${GLAD_INCLUDE_DIR} ${GLM_INCLUDE_DIR})  # Указываем GLM

target_link_libraries(Lab_04 ${OPENGL_gl_LIBRARY} glu32)

# Подключение GLUT (если используется)
find_package(GLUT REQUIRED)
