#pragma once

#include <iostream>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <random>
#include "../common/bmp.h"

typedef struct line_t {
    int p; // 极径

    int theta; // 极角

};

class hough_transformation {
private:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp bmp_file_24_;        // 24位bmp文件
    bmp save_file_;          // 处理完的bmp文件
    int32_t width_;          // 图像宽度
    int32_t height_;         // 图像高度
    int threshold_t_;        // 阈值
    std::vector<line_t> lines_t_; // 达到阈值的线的个数

    void draw_line(uint8_t* data, line_t line);

public:
    hough_transformation(std::string file_path) : file_path_(file_path) {
        threshold_t_ = 100;
    }

    void read_file();

    void convert_8_to_24();

    void execute_transformation();

};