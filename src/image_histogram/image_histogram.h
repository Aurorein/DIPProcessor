#pragma once

#include "../common/bmp.h"

class image_histogram{
public:
    image_histogram(std::string file_path) : file_path_(file_path){}

    void set_image_path(std::string file_path){file_path_ = file_path;}

    void read_file();

    void get_histogram_file();

    void get_histogram_equaled();

private:
    uint32_t find_max_in_array(uint32_t arr[], uint32_t size_t);

    void mapping_gray(float* frequen_accum, uint8_t* histogram_data, uint8_t* equal_histogram_data);

    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp histogram_file_;          // 输入的图像的直方图文件
    uint32_t *piex_arr_;        // 灰度频度数组
    bmp equal_histogram_file_;    // 均衡化处理后的直方图文件
};