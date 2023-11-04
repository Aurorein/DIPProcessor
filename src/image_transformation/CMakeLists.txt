# 添加源文件
set(SOURCES image_transformation.cpp)

# 生成静态库
add_library(image_transformation STATIC ${SOURCES})