cmake_minimum_required(VERSION 3.16)
project(imageProcess)

add_executable(image-server main.cpp image_process.cpp)

target_include_directories(image-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})