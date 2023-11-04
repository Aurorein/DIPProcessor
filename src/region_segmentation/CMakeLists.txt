# 添加源文件
set(SOURCES region_growing.cpp region_spliting.cpp)

# 生成静态库
add_library(region_segmentation STATIC ${SOURCES})