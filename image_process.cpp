#include "image_process.h"
#include <fstream>


void image_process::read_file(){

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
    // 如果颜色位数是24，说明没有用到调色板
    if(bmp_file_.get_bit_count() == 8){

    }

    // 3. 读取真实数据
    char *data = new char[bmp_file_.get_size_image()];
    bmp_file_.set_data(data);
    file.read(data, bmp_file_.get_size_image());


    // 

    // 关闭文件资源
    file.close();
}



void image_process::convert_to_gray(){
    // 1. 计算灰度值
    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();
    int32_t bit_count = bmp_file_.get_bit_count();
    // 先计算lineByte的原因是位图文件头和位图信息头的更改都跟它有关
    int32_t line_bytes_bmp = (width * bit_count / 8 + 3) / 4 * 4;
    int32_t line_bytes_save = (width * 8 / 8 + 3) / 4 * 4;

    // 2. 另存为bmp文件
    save_file_.set_file_header(bmp_file_.get_file_header());
    save_file_.set_info_header(bmp_file_.get_info_header());

    // 修改位图文件头
    save_file_.set_bf_size(FILEHEADER_SIZE + INFOHEDER_SIZE + 256*sizeof(RGBQUAD) + line_bytes_save*height);
    save_file_.set_bf_off_bits(FILEHEADER_SIZE + INFOHEDER_SIZE + 256*sizeof(RGBQUAD));

    // 修改位图信息头
    save_file_.set_bit_count(8);
    save_file_.set_size_image(line_bytes_save * height);

    // 在当前file_path下加一个_gray后缀
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    file_name += "_gray";
    std::string save_file_path = file_name + "." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream file(save_file_path, std::ios::binary);
    if (!file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }
    BITMAPFILEHEADER file_header = save_file_.get_file_header();
    BITMAPINFOHEADER info_header = save_file_.get_info_header();
    file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 3. 写入调色板
    RGBQUAD* palette = new RGBQUAD[256];
    save_file_.set_queue(palette);
    for(int32_t i = 0;i< 256;i++){
        palette[i].rgb_blue_ = i;
        palette[i].rgb_green_ = i;
        palette[i].rgb_red_ = i;
        palette[i].rgb_reserved_ = 0;
    }
    file.write(reinterpret_cast<const char*>(palette), sizeof(RGBQUAD)*(256));

    // 4. 写入位图数据
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    // 创建生成灰度图像后的数据矩阵，计算灰度值数据矩阵
    uint8_t* gray_data = new uint8_t[height*line_bytes_save];
    
    for(int32_t i = 0; i < height; i++){
        for(int32_t j = 0; j< width;j++){
            uint8_t b = data[i*line_bytes_bmp + 3*j];
            uint8_t g = data[i*line_bytes_bmp + 3*j+1];
            uint8_t r = data[i*line_bytes_bmp + 3*j+2];
            uint8_t gray =  (299 * r + 587 * g + 114 * b)/ 1000;
            gray_data[i*line_bytes_save + j] = gray;
        }
    }
    // uint16_t *save_data = new uint16_t[width * height];
    file.write(reinterpret_cast<const char*>(gray_data), sizeof(uint8_t)*(height * line_bytes_save));
    
    // 关闭文件资源
    file.close();
}