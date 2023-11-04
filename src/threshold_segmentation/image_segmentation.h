#pragma once

#include <iostream>
#include <string.h>
#include <fstream>
#include "../common/bmp.h"

class image_segmentation{
private:
    bmp save_file_;          // 经过图像分割处理的bmp文件
    bmp histogram_file_;          // 输入的图像的直方图文件
    uint32_t *piex_arr_;        // 灰度频度数组

    uint32_t find_max_in_array(uint32_t arr[], uint32_t size_t);
protected:
    uint8_t threshold_;       // 阈值
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件

public:
    void set_threshold(uint8_t threshold){threshold_ = threshold;}

    image_segmentation(std::string file_path) :file_path_(file_path){};

    void read_file();
        
    virtual void get_threshold(){return;};

    void output_segmentation_res();

    virtual std::string get_type(){return "";}

};