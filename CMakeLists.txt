cmake_minimum_required(VERSION 3.19)
project(phantom)
set(CMAKE_CXX_STANDARD 17)
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.1)

FetchContent_MakeAvailable(Catch2)

include_directories(Source/Public)

add_executable(phantom main.cpp Source/Private/Engine.cpp Source/Private/Sprite.cpp Source/Private/Vector3.cpp)

target_link_libraries(phantom SDL2main SDL2 GLEW GLU GL PRIVA)
