cmake_minimum_required(VERSION 3.19)
project(phantom)
set(CMAKE_CXX_STANDARD 17)

include_directories(Source/Public)

# Setup Testing - Need to setup separate project for testing.
#find_package(Catch2 REQUIRED)
#include(CTest)
#include(Catch)
#add_test(Tests/LoggerTest.cpp)
#target_link_libraries(Catch2::Catch2)
#End Testing

add_executable(phantom main.cpp Source/Private/Engine.cpp Source/Private/Sprite.cpp Source/Private/Vector3.cpp)

target_link_libraries(phantom SDL2main SDL2 GLEW GLU GL)