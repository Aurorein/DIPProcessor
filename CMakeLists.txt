cmake_minimum_required(VERSION 3.16)
project(imageProcess)
set(CMAKE_BUILD_TYPE "Debug")

# 添加头文件搜索路径
include_directories(src/image_process)
include_directories(src/image_histogram)
include_directories(src/spatial_domain_filtering)
include_directories(src/image_transformation)

# 添加源文件
add_subdirectory(src/image_process)
add_subdirectory(src/image_histogram)
add_subdirectory(src/spatial_domain_filtering)
add_subdirectory(src/image_transformation)
add_subdirectory(src/threshold_segmentation)

# target_include_directories(image-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(image-server main.cpp)
target_link_libraries(image-server image_process image_histogram spatial_domain_filtering image_transformation image_segmentation)