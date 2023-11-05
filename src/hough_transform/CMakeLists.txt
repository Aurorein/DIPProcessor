# 添加源文件
set(SOURCES hough_transform.cpp)

# 生成静态库
add_library(hough_transform STATIC ${SOURCES})