#include <iostream>
#include <fstream>
#include "../common/bmp.h"

class contour_extraction {
private:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp save_file_;          // 处理后的bmp文件
    int32_t width_;
    int32_t height_;

    bool is_inner_point(uint8_t* data, int x, int y);
    

public:
    contour_extraction(std::string file_path) : file_path_(file_path) {
    }

    void read_file();

    void execute_contour_extract();

};