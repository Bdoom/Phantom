cmake_minimum_required(VERSION 3.19)
project(PhantomTests)
set(CMAKE_CXX_STANDARD 17)

include_directories(../Source/Public)

# Setup Testing - Need to setup separate project for testing.
find_package(Catch2 REQUIRED)
include(Catch)
enable_testing()
add_executable(PhantomTests TestingMain.cpp LoggerTest.cpp)
target_link_libraries(PhantomTests PRIVATE Catch2::Catch2)
catch_discover_tests(PhantomTests)
#End Testing Setup