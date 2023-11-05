# 添加源文件
set(SOURCES contour_extraction.cpp)

# 生成静态库
add_library(contour_extraction STATIC ${SOURCES})