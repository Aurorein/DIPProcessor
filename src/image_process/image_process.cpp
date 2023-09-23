#include "image_process.h"
#include <fstream>
#include <cstring>


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



void image_process::convert_to_gray(){
    // 判断是否为24真彩图片
    if(bmp_file_.get_bit_count() != 24){
        std::cerr<<"please input a 24 color image!"<<std::endl;
        return;
    }

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


void image_process::inverse_gray_color(){
    if(bmp_file_.get_bit_count() != 8){
        std::cerr<<"please input a gray image!"<<std::endl;
        return;
    }

    // 1. 在当前的file_path下加一个_inverse
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    file_name += "_inverse";
    std::string save_file_path = file_name + "." + file_ext;

    // 2. 拷贝原来文件的位图文件头、位图信息头、调色板
    std::ofstream file(save_file_path, std::ios::binary);
    if (!file) {
        std::cerr << "failed to create the BMP file to save." << std::endl;
        return;
    }

    BITMAPFILEHEADER file_header = bmp_file_.get_file_header();
    BITMAPINFOHEADER info_header = bmp_file_.get_info_header();
    file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);
    // 拷贝queue
    int32_t size_of_queue = sizeof(RGBQUAD)*256;
    file.write(reinterpret_cast<const char*>(bmp_file_.get_queue()), size_of_queue);

    // 3. 对每个位图数据取反
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();
    uint8_t* inverse_data = new uint8_t[bmp_file_.get_size_image()];
    int32_t byte_lines = (width * 8 / 8 + 3) / 4 * 4;
    
    for(int32_t i = 0;i<height ;i++){
        for(int32_t j = 0;j<width;j++){
            *(inverse_data + i*byte_lines + j) = 255 - *(data + i*byte_lines + j);
        }
    }
    file.write(reinterpret_cast<char *>(inverse_data), bmp_file_.get_size_image());
    
    // 记得释放资源
    delete[] inverse_data;
    
    // 关闭文件资源
    file.close();

}

void image_process::channel_split(){
    // 判断是否为24真彩图片
    if(bmp_file_.get_bit_count() != 24){
        std::cerr<<"please input a 24 color image!"<<std::endl;
        return;
    }

    // 1. 计算灰度值
    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();
    int32_t bit_count = bmp_file_.get_bit_count();
    // 先计算lineByte的原因是位图文件头和位图信息头的更改都跟它有关
    int32_t line_bytes_bmp = (width * bit_count / 8 + 3) / 4 * 4;
    int32_t line_bytes_save = (width * 8 / 8 + 3) / 4 * 4;

    // 暂存文件头到save_file
    save_file_.set_file_header(bmp_file_.get_file_header());
    save_file_.set_info_header(bmp_file_.get_info_header());

    // 修改位图文件头
    save_file_.set_bf_size(FILEHEADER_SIZE + INFOHEDER_SIZE + 256*sizeof(RGBQUAD) + line_bytes_save*height);
    save_file_.set_bf_off_bits(FILEHEADER_SIZE + INFOHEDER_SIZE + 256*sizeof(RGBQUAD));

    // 修改位图信息头
    save_file_.set_bit_count(8);
    save_file_.set_size_image(line_bytes_save * height);

    // 位图文件头和位图信息头写入file
    BITMAPFILEHEADER file_header = save_file_.get_file_header();
    BITMAPINFOHEADER info_header = save_file_.get_info_header();

     // 生成8位调色板
    RGBQUAD* palette = new RGBQUAD[256];
    save_file_.set_queue(palette);
    for(int32_t i = 0;i< 256;i++){
        palette[i].rgb_blue_ = i;
        palette[i].rgb_green_ = i;
        palette[i].rgb_red_ = i;
        palette[i].rgb_reserved_ = 0;
    }

    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());

    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    // 先生成R通道的图像

    // 在当前file_path下加一个_r后缀
    std::string save_r_file_path = file_name + "_r." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream r_file(save_r_file_path, std::ios::binary);
    if (!r_file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }   

    // 写入R通道位图信息头和位图文件头
    r_file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    r_file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 写入调色板
    r_file.write(reinterpret_cast<const char*>(palette), sizeof(RGBQUAD)*(256));

    // 对每个data区处R通道
    uint8_t* r_data = new uint8_t[bmp_file_.get_size_image()];
    
    for(int32_t i = 0; i < height; i++){
        for(int32_t j = 0; j< width;j++){
            uint8_t r = data[i*line_bytes_bmp + 3*j+2];
            r_data[i*line_bytes_save + j] = r;
        }
    }
    r_file.write(reinterpret_cast<const char*>(r_data), sizeof(uint8_t)*(height * line_bytes_save));

    delete[] r_data;
    r_file.close();

    // 先生成G通道的图像

    // 在当前file_path下加一个_g后缀
    std::string save_g_file_path = file_name + "_g." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream g_file(save_g_file_path, std::ios::binary);
    if (!g_file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }   

    // 写入R通道位图信息头和位图文件头
    g_file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    g_file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 写入调色板
    g_file.write(reinterpret_cast<const char*>(palette), sizeof(RGBQUAD)*(256));

    // 对每个data区处R通道
    uint8_t* g_data = new uint8_t[bmp_file_.get_size_image()];
    
    for(int32_t i = 0; i < height; i++){
        for(int32_t j = 0; j< width;j++){
            uint8_t g = data[i*line_bytes_bmp + 3*j+1];
            g_data[i*line_bytes_save + j] = g;
        }
    }
    g_file.write(reinterpret_cast<const char*>(g_data), sizeof(uint8_t)*(height * line_bytes_save));

    delete[] g_data;
    g_file.close();

    // 先生成B通道的图像

    // 在当前file_path下加一个_b后缀
    std::string save_b_file_path = file_name + "_b." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream b_file(save_b_file_path, std::ios::binary);
    if (!b_file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }   

    // 写入R通道位图信息头和位图文件头
    b_file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    b_file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 写入调色板
    b_file.write(reinterpret_cast<const char*>(palette), sizeof(RGBQUAD)*(256));

    // 对每个data区处R通道
    uint8_t* b_data = new uint8_t[bmp_file_.get_size_image()];
    
    for(int32_t i = 0; i < height; i++){
        for(int32_t j = 0; j< width;j++){
            uint8_t b = data[i*line_bytes_bmp + 3*j];
            b_data[i*line_bytes_save + j] = b;
        }
    }
    b_file.write(reinterpret_cast<const char*>(b_data), sizeof(uint8_t)*(height * line_bytes_save));

    delete[] b_data;
    b_file.close();



}