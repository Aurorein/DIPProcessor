#include "bmp.h"


const int FILEHEADER_SIZE = sizeof(BITMAPFILEHEADER);
const int INFOHEDER_SIZE = sizeof(BITMAPINFOHEADER);

class image_process{
public:
    void read_file(std::string file_path);

private:
    bmp bmp_file_;

};