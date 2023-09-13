#pragma once

#include "../common/bmp.h"

class image_histogram{
public:
    image_histogram(std::string file_path) : file_path_(file_path){}

    void read_file();

    void get_histogram_file();

private:
    uint8_t find_max_in_array(uint8_t arr[], uint32_t size_t);

    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp histogram_file_;          // 输入的图像的直方图文件
    bmp equal_histogram_file_;    // 均衡化处理后的直方图文件
};