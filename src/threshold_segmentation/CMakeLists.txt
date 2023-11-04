# 添加源文件
set(SOURCES image_segmentation.cpp)

# 生成静态库
add_library(image_segmentation STATIC ${SOURCES})