#pragma once

#include "../common/bmp.h"
#include "../common/convolution_mask/convolution_mask.h"
#include <fstream>
#include <cstring>

class spatial_domain_filtering{
public:
    spatial_domain_filtering(std::string file_path) : file_path_(file_path){}

    void read_file();

    virtual void do_filtering(int border);

    void set_mask(convolution_mask* mask){this->mask_ = mask;}

    convolution_mask* get_mask(){return mask_;}

    int get_border(){return border_;}

    void set_border(int border){this->border_ = border;}

    virtual std::string get_type(){return "";}
private:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    convolution_mask* mask_; // 使用的滤波类型
    int border_;             // 卷积边界
};