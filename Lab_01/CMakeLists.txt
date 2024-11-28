cmake_minimum_required(VERSION 3.10)

project(Lab_01)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:/C++_libs/SFML-2.6.1/lib/cmake/SFML")


add_executable(Lab_01 src/main.cpp)


find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(Lab_01 sfml-graphics sfml-window sfml-system)
