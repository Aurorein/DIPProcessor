# 添加源文件
set(SOURCES image_process.cpp)

# 生成静态库
add_library(image_process STATIC ${SOURCES})