#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "../common/bmp.h"

class region_growing {
private:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
    bmp save_file_;          // 处理后的bmp文件
    std::vector<int*> coors_;    // 要处理的坐标集


public:
    region_growing(std::string file_path, std::vector<int*> coors) {
        file_path_ = file_path; 
        for(int* c : coors){
            coors_.emplace_back(c);
        }    
    }

    void read_file();

    void excute_growing();

    void dfs(bool** vis, uint8_t* data, int i, int j);

};