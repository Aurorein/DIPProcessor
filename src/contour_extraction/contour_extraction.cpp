#include "contour_extraction.h"


void contour_extraction::read_file() {
    std::ifstream file(file_path_, std::ios::binary);
        if (!file) {
            std::cout << "failed to read the BMP file." << std::endl;
            return;
        }

        // 1. 读取位图文件头和信息头
        char* header_buffer = new char[FILEHEADER_SIZE + INFOHEDER_SIZE];
        file.read(header_buffer, FILEHEADER_SIZE + INFOHEDER_SIZE);

        bmp_file_.deserialize_file_header(header_buffer);
        bmp_file_.deserialize_info_header(header_buffer + FILEHEADER_SIZE);
        delete header_buffer;

        // 2. 如果有调色板，读取调色板
        // 如果颜色位数是8
        if(bmp_file_.get_bit_count() == 8){
            int32_t size_of_queue = sizeof(RGBQUAD)*(256);

            char* queue_buffer = new char[size_of_queue];
            file.read(queue_buffer, size_of_queue);

            
            bmp_file_.set_queue(reinterpret_cast<RGBQUAD *>(queue_buffer));
            
        }
        

        // 3. 读取真实数据
        int32_t size = bmp_file_.get_size_image();
        if(size == 0){
            int32_t height = bmp_file_.get_height();
            int32_t width = bmp_file_.get_width();
            int16_t bit_count = bmp_file_.get_bit_count();
            int32_t line_bytes_bmp = (width * bit_count / 8 + 3) / 4 * 4;

            size = line_bytes_bmp * height;

        }
        char *data = new char[bmp_file_.get_size_image()];
        bmp_file_.set_data(data);
        file.seekg(bmp_file_.get_bf_off_bits(), std::ios::beg);
        file.read(data, bmp_file_.get_size_image());

        // 
        // 关闭文件资源
        file.close();
}

void contour_extraction::execute_contour_extract() {
    int32_t width = bmp_file_.get_width();
    width_ = width;
    int32_t height = bmp_file_.get_height();
    height_ = height;

    // 对原图像数据进行扩充
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;
    uint8_t* data_save = new uint8_t[line_bytes * height];
    
    // 遍历每一个元素
    for(int i = 0; i < height; ++i) {
        for(int j = 0 ; j < width; ++j) {
            if(*(data + i * line_bytes + j) == 0){
                if(is_inner_point(data, i, j)){
                    *(data_save + i * line_bytes + j) = 255;
                }else{
                    *(data_save + i * line_bytes + j) = 0;
                }
            }else if(*(data + i * line_bytes + j) == 255){
                *(data_save + i * line_bytes + j) = 255;
            }
            

        }
    }

    // 将数据写入文件
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    file_name += "_contour_extraction";
    std::string save_file_path = file_name + "." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream file(save_file_path, std::ios::binary);
    if (!file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }

    // 将文件位图信息头和位图文件头写入磁盘
    BITMAPFILEHEADER file_header = bmp_file_.get_file_header();
    BITMAPINFOHEADER info_header = bmp_file_.get_info_header();
    file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 将位图板写入磁盘
    file.write(reinterpret_cast<const char*>(bmp_file_.get_queue()), sizeof(RGBQUAD)*(256));

    // 将位图数据写入磁盘
    file.write(reinterpret_cast<const char*>(data_save), sizeof(uint8_t)*(line_bytes * height));

    // 释放资源
    // delete[] data;

    // 关闭文件资源
    file.close();


}


bool contour_extraction::is_inner_point(uint8_t* data, int x, int y) {
    int32_t line_bytes = (width_ * 8 / 8 + 3) / 4 * 4;

    if(*(data + x * line_bytes + y) != 0){
        return false;
    }
    int coor[5] = {0, 1, 0, -1, 0};
    int coor_t[5] = {-1, 1, 1, -1, -1};
    // 上下左右
    for(int k = 0 ; k < 4; ++k) {
        int xx = x + coor[k];
        int yy = y + coor[k + 1];
        if(xx < 0 || xx > height_ || yy < 0 || yy > width_) {
            continue;
        }
        if(*(data + xx * line_bytes + yy) != 0){
            return false;
        }
    }
    // 四个斜角
    for(int k = 0; k < 4; ++k){
        int xx = x + coor_t[k];
        int yy = y + coor_t[k + 1];
        if(xx < 0 || xx > height_ || yy < 0 || yy > width_) {
            continue;
        }
        if(*(data + xx * line_bytes + yy) != 0){
            return false;
        }
    }

    return true;
}