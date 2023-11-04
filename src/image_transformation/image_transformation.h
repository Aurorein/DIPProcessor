#pragma once

#include "../../src/common/bmp.h"
#include "../../src/common/transformational_matrix/transformational_matrix.h"

class image_transformation{
public:
    image_transformation(std::string file_path) : file_path_(file_path){}

    void read_file();

    void transformation();

    void set_matrix(transformational_matrix* matrix){matrix_ = matrix;}

    uint8_t interpolation(coordinate point);

    virtual std::string get_type(){return "";}
private:
    bmp bmp_file_;          // 处理后要保存的bmp文件
    std::string file_path_;  // 要处理的文件路径
    transformational_matrix *matrix_;   // 要做变换的矩阵

};