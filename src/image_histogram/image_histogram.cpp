#include "image_histogram.h"
#include <fstream>
#include <cstring>

void image_histogram::read_file(){
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
        
    }else{
        std::cerr<<"please input a gray image in this function"<<std::endl;
        exit(1);
    }

    // 3. 读取真实数据
    char *data = new char[bmp_file_.get_size_image()];
    bmp_file_.set_data(data);
    file.seekg(bmp_file_.get_bf_off_bits(), std::ios::beg);
    file.read(data, bmp_file_.get_size_image());


    // 

    // 关闭文件资源
    file.close();
}

void image_histogram::get_histogram_file(){
    // 直方图使用灰度图像，可以照搬bmp_file（一定是灰度图像）的头信息
    // 这里可以优化的是只在数位板中使用两种颜色，这里将256种颜色全部照搬过来了
    histogram_file_.set_file_header(bmp_file_.get_file_header());
    histogram_file_.set_info_header(bmp_file_.get_info_header());

    // 因为我们要生成的直方图图像是256*256，需要修改一些图像位图头信息
    histogram_file_.set_height(256);
    histogram_file_.set_width(256);

    int line_bytes_histogram = (256 * 8 / 8 + 3) / 4 * 4;

    // 修改位图文件头和信息头
    histogram_file_.set_bf_size(FILEHEADER_SIZE + INFOHEDER_SIZE + 256*sizeof(RGBQUAD) + line_bytes_histogram*256);
    // histogram_file_.set_bf_off_bits(FILEHEADER_SIZE + INFOHEDER_SIZE + 256*sizeof(RGBQUAD));
    histogram_file_.set_size_image(line_bytes_histogram * 256);

    histogram_file_.set_queue(bmp_file_.get_queue());

    // 声明一个长度为256的数组，用来统计每个灰度的频度
    uint32_t *frequen_array = new uint32_t[256]{0};

    // 对灰度图每一个位图数据byte，将其统计到灰度频度数组中
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();
    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;

    for(uint32_t i = 0; i < height; i++){
        for(uint32_t j = 0 ;j < width; j++){
            uint32_t gray_data = *(data + i*line_bytes + j);
            frequen_array[gray_data] ++;
        }
    }

    piex_arr_ = new uint32_t[256];
    memcpy(piex_arr_, frequen_array, 256 * sizeof(uint32_t));
    // 创建二维数组，对要保存的直方图的位图数据进行填充

    // 1. 首先进行位图数据的归一化
    uint32_t max_frequen = find_max_in_array(frequen_array ,256);
    float frequen_scale = (float)max_frequen / 256;
    for(uint32_t i = 0; i < 256 ; i++){
        frequen_array[i] = (uint8_t)( frequen_array[i] / frequen_scale );
    }

    // 2. 将位图数据填充
    uint32_t size_image = histogram_file_.get_size_image();
    uint8_t histogram_data[size_image];

    // width
    for(uint32_t i = 0;i<256;i++){
        // height
        for(uint32_t j = 0;j <256;j++){
            if(frequen_array[i] > 0){
                *(histogram_data + j*line_bytes_histogram + i) = 235;
                frequen_array[i] --;
            }else{
                *(histogram_data + j*line_bytes_histogram + i) = 0;
            }
        }
    }

    histogram_file_.set_data(reinterpret_cast<char *>(histogram_data));

    // 将数据写入文件
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    file_name += "_histogram";
    std::string save_file_path = file_name + "." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream file(save_file_path, std::ios::binary);
    if (!file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }

    // 将文件位图信息头和位图文件头写入磁盘
    BITMAPFILEHEADER file_header = histogram_file_.get_file_header();
    BITMAPINFOHEADER info_header = histogram_file_.get_info_header();
    file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 将位图板写入磁盘
    file.write(reinterpret_cast<const char*>(bmp_file_.get_queue()), sizeof(RGBQUAD)*(256));

    // 将位图数据写入磁盘
    file.write(reinterpret_cast<const char*>(histogram_data), sizeof(uint8_t)*(size_image));

    // 释放资源
    delete[] data;

    // 关闭文件资源
    file.close();

}

void image_histogram::get_histogram_equaled(){
    // 定义好需要的数组
    float sum_piex = 0.0;
    uint32_t size_image = bmp_file_.get_size_image();
    uint8_t *bmp_data = reinterpret_cast<uint8_t *>(bmp_file_.get_data());


    uint32_t piex_accum[256];
    float frequen_accum[256];
    uint32_t tmp_accum = 0;
    // 得到像素点累加数组
    for(uint32_t i = 0;i< 256;i++){
        piex_accum[i] += tmp_accum + piex_arr_[i]; 
        tmp_accum = piex_accum[i];
    }
    // 由总面积（所有像素点）得到灰度频度累加数组
    sum_piex = (float)piex_accum[255];
    for(uint32_t j = 0;j<256;j++){
        frequen_accum[j] = piex_accum[j] / sum_piex;
    }

    // 声明一个数组用来填充均衡化之后的位图数据
    uint8_t *equal_histogram_data = new uint8_t[size_image];

    // 进行灰度映射
    mapping_gray(frequen_accum, bmp_data, equal_histogram_data);

    // 另存为文件
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    file_name += "_equal";
    std::string save_file_path = file_name + "." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream file(save_file_path, std::ios::binary);
    if (!file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }

    // 写入位图文件头和信息头
    // 将文件位图信息头和位图文件头写入磁盘
    BITMAPFILEHEADER file_header = bmp_file_.get_file_header();
    BITMAPINFOHEADER info_header = bmp_file_.get_info_header();
    file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 将位图板写入磁盘
    file.write(reinterpret_cast<const char*>(bmp_file_.get_queue()), sizeof(RGBQUAD)*(256));

    // 将位图数据写入磁盘
    file.write(reinterpret_cast<const char*>(equal_histogram_data), sizeof(uint8_t)*(size_image));

    // 释放资源
    delete[] equal_histogram_data;

    // 关闭文件资源
    file.close();

     /**
     * 为了实现第三题同时得到直方图
     */
    set_image_path(save_file_path);

    read_file();

    get_histogram_file();

}


uint32_t image_histogram::find_max_in_array(uint32_t arr[], uint32_t size_t){
    uint32_t max = 0;
    for(uint32_t i = 0 ; i < size_t ; i++){
        if(arr[i] > max){
            max = arr[i];
        }
    }
    return max;
}

void image_histogram::mapping_gray(float* frequen_accum, uint8_t* bmp_data, uint8_t* equal_histogram_data){

    uint8_t mapping_arr[256];
    // 映射到[0-255]  int[(L-1)Sk + 0.5]
    for(uint32_t i = 0; i < 256 ;i++){
        mapping_arr[i] = uint8_t(255 * frequen_accum[i] + 0.5);
    }

    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();
    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;

    // 灰度值映射
    for(int32_t i = 0; i< height ;i++){
        for(int32_t j = 0; j< width ;j++){
            uint32_t origin_data = (uint32_t)*(bmp_data + i*line_bytes + j);
            *(equal_histogram_data + i*line_bytes + j) = mapping_arr[origin_data];
        }
    }

}