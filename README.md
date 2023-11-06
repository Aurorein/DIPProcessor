# Image_process
image process class homework

2023年哈尔滨工业大学（威海）数字图像处理课程作业
C++不使用非标准库实现基本的数字图像处理的原理
程序提供10个标准功能，以及控制台标准输入的交互界面

Dev环境：
WSL2.0:Ubuntu-20.04  GCC9.4.0 x86_64-linux-gnu Cmake

语言：C++

### 目录结构：
##### src
###### image_process (作业1 图像变换)
###### image_histogram (作业2 图像直方图)
###### spatial_domain_filtering（作业3 空间滤波）
###### image_transformation（作业4 图像变换）
###### threshold_segmentation（作业5 阈值分割）
###### region_segmentation（作业6 区域分割）
###### edge_election（作业7 边缘检测）
###### hough_transformation（作业8 hough变换）
###### zone_marker（作业9 区域标记）
###### contour_extraction（作业10 轮廓提取）

### 运行
mkdir build  
cd build  
cmake ..  
cmake --build .  
./image-server 要处理的图像的路径  
