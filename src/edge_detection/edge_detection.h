#pragma once

#include <fstream>
#include <cstring>
#include "../common/bmp.h"
#include "../common/edge_detection_operator/edge_detection_operator.h"

class edge_detection {
protected:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp save_file_;          // 处理后的bmp文件
    edge_detection_operator* oper_; // 边缘检测算子


public:
    edge_detection(std::string file_path) : file_path_(file_path) {}

    void read_file();

    void execute_detection();

    virtual std::string get_type(){return "";}
};