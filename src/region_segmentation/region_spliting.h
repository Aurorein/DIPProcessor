#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "../common/bmp.h"

class region_spliting{
private:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp save_file_;          // 处理后的bmp文件

public:
    region_spliting(std::string file_path) : file_path_(file_path) {
    }

    void read_file();

    void execute_spliting();

    void dfs(bool** vis, uint8_t* data, int i1, int i2, int j1, int j2);

    bool match_distance(uint8_t* data, int i1, int i2, int j1, int j2);

};