# 添加源文件
set(SOURCES image_histogram.cpp)

# 生成静态库
add_library(image_histogram STATIC ${SOURCES})