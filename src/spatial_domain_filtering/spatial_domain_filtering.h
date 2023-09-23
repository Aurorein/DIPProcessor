#include "../common/bmp.h"
#include <fstream>
#include <cstring>

class spatial_domain_filtering{
public:
    spatial_domain_filtering(std::string file_path) : file_path_(file_path){}

    void read_file();

    void do_smoothing(int border);
private:
    std::string file_path_;  // 要处理的文件路径
    bmp bmp_file_;           // 待处理的bmp文件
};