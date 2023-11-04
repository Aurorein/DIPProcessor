#include "image_transformation.h"
#include <fstream>
#include <string>
#include <cmath>

void image_transformation::read_file(){
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

void image_transformation::transformation(){
    // 目前只支持灰度图像的变换
    uint16_t bit_count = bmp_file_.get_bit_count();
    if(bit_count != 8){
        throw std::exception();
    }

    // 1.开辟一块相同大小的位图数据数组，定义必要的变量
    int32_t height = bmp_file_.get_height();
    int32_t width = bmp_file_.get_width();
    int32_t line_bytes = (width *bit_count / 8 + 3) / 4 * 4;
    int32_t image_size = bmp_file_.get_size_image();

    uint8_t *data = reinterpret_cast<uint8_t *>(bmp_file_.get_data());
    uint8_t *data_transform = new uint8_t[image_size];

    // 2.对data_transform中每一位(x,y)，通过逆向变换矩阵得到原data中的(x,y)
    for(int32_t y_t = 0; y_t < height; y_t ++){
        for(int32_t x_t = 0; x_t < width ; x_t ++){
            coordinate point(x_t, y_t, 1);
            coordinate point_transform = matrix_->compute(point);

            // 若该点位于原图像之外
            if(point_transform.x_ < 0 || point_transform.x_ > height || point_transform.y_ < 0 || point_transform.y_ > width){
                point_transform.x_ = point_transform.y_ = 0;
                continue;
            }

            uint8_t data_t = interpolation(point_transform);
            *(data_transform + y_t * line_bytes + x_t) = data_t;
        }
    }

    // 将新的data写入文件
    // 在当前file_path下加一个_average_filtering后缀
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);


    file_name += "_" + get_type() + "_transformation";
    std::string save_file_path = file_name + "." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream file(save_file_path, std::ios::binary);
    if (!file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }

    // 将文件头和调色板的信息写入
    BITMAPFILEHEADER file_header = bmp_file_.get_file_header();
    BITMAPINFOHEADER info_header = bmp_file_.get_info_header();
    file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    file.write(reinterpret_cast<const char*>(bmp_file_.get_queue()), sizeof(RGBQUAD)*(256));

    // 将新的文件数据写入
    file.write(reinterpret_cast<const char*>(data_transform), sizeof(uint8_t)*(image_size));

    // 释放文件资源
    file.close();
}

uint8_t image_transformation::interpolation(coordinate point){
    float x = point.x_;
    float y = point.y_;

    // 获取边界点
    // 获取x的两个边界
    int x_floor = floor(x);
    int x_ceil = x_floor + 1;
    // 获取y的两个边界
    int y_floor = floor(y);
    int y_ceil = y_floor + 1;

    // 四个点分别为 (x_floor, y_floor), (x_ceil, y_floor), (x_floor, y_ceil), (x_ceil, y_ceil)
    uint8_t *data = reinterpret_cast<uint8_t *>(bmp_file_.get_data());
    int32_t line_bytes = (bmp_file_.get_width() *bmp_file_.get_bit_count() / 8 + 3) / 4 * 4;;
    // (x_floor, y_floor)
    uint8_t data_xfyf = *(data + y_floor * line_bytes + x_floor);
    // (x_ceil, y_floor)
    uint8_t data_xcyf = *(data + y_floor * line_bytes + x_ceil);
    // (x_floor, y_ceil)
    uint8_t data_xfyc = *(data + y_ceil * line_bytes + x_floor);
    // (x_ceil, y_ceil)
    uint8_t data_xcyc = *(data + y_ceil * line_bytes + x_ceil);

    // 双线性插值
    float floor_val = data_xfyf + (data_xcyf - data_xfyf) * (x - x_floor);
    float ceil_val = data_xfyc + (data_xcyc - data_xfyc) * (x - x_floor);
    float data_transfrom_f = ceil_val + (ceil_val - floor_val) * (y - y_floor);

    uint8_t data_transfrom;
    if(data_transfrom_f > 255){
        data_transfrom = 255;
    }else if(data_transfrom_f < 0){
        data_transfrom = 0;
    }else{
        data_transfrom = (uint8_t)data_transfrom_f;
    }

    return data_transfrom;
}
