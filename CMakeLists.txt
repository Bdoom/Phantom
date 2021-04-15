cmake_minimum_required(VERSION 3.19)
project(phantom)
set(CMAKE_CXX_STANDARD 14)

include_directories(Source/Public)

add_executable(phantom main.cpp Source/Private/Engine.cpp Source/Public/Engine.h Source/Public/IniReader.h Source/Public/Logger.h)

target_link_libraries(phantom SDL2main SDL2)