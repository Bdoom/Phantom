cmake_minimum_required(VERSION 3.19)
project(phantom)
set(CMAKE_CXX_STANDARD 14)

include_directories(Source/Public)

add_executable(phantom main.cpp Source/Private/Engine.cpp Source/Private/Sprite.cpp Source/Private/Vector3.cpp)

target_link_libraries(phantom SDL2main SDL2 GLEW GLU GL)
