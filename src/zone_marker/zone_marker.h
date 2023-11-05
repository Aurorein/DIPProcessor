#include <iostream>
#include <fstream>
#include <random>
#include "../common/bmp.h"

class zone_marker {
private:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp bmp_file_24_;        // 24位bmp文件
    bmp save_file_;          // 处理完的bmp文件
    int32_t width_;          // 图像宽度
    int32_t height_;         // 图像高度
    short** vis_;             // 连通图，0表示为遍历，1表示遍历到，2表示已经上颜色

    void dfs(uint8_t* data, int x, int y);

    void dye();

public:
    zone_marker(std::string file_path) : file_path_(file_path){}

    void read_file();

    void convert_8_to_24();

    void execute_zone_mark();

};
