#include "hough_transform.h"


void hough_transformation::read_file(){
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

void hough_transformation::execute_transformation() {
    // 图像基本信息
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t width = bmp_file_.get_width();
    width_ = width;
    int32_t height = bmp_file_.get_height();
    height_ = height;

    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;
    int32_t line_bytes_24 = (width * 24 / 8 + 3) / 4 * 4;

    int32_t s_t;
    int32_t offset_t;

    // 计算size_t
    s_t = width * width + height * height;
    s_t = 2 * sqrt(s_t) + 100;
    // 计算offset_t
    offset_t = s_t / 2;

    // 计数板
    int** ct_board = new int*[s_t];
    for(int i = 0; i < s_t; ++i) {
        ct_board[i] = new int[181]{0};
    }

    if(!ct_board){perror("error to alloc mem for ct_board!");}

    // 对每个像素点统计出现次数
    int p = 0;
    for(int x = 0 ; x < height; ++x) {
        for(int y = 0; y < width; ++y) {
            uint8_t d = *(data + x * line_bytes + y);

            // 检测黑线
            if(d == 0) {
                for(int agl = 0; agl < 181; agl ++) {
                    p = x * cos(agl * M_PI / 180.0) + y * sin(agl * M_PI / 180.0) + offset_t;
                
                    if(p < 0 || p >= s_t){continue;}

                    ct_board[p][agl] ++;
                }
            }
        }
    }

    // 遍历ct_board，选出符合阈值的直线
    int ct = 0;
    int max = 0, mp =0, mt = 0;
    for(int i = 0 ; i < s_t; ++i) {
        for(int j  = 0; j < 181; j ++) {
            if(ct_board[i][j] > max) {
                max = ct_board[i][j];
                mp = i - offset_t;
                mt = j;
            }

            if(ct_board[i][j] >= threshold_t_){
                lines_t_.emplace_back(line_t{i- offset_t, j});
            }
        }

    }

    // 绘制直线
    uint8_t* data_save = reinterpret_cast<uint8_t*>(bmp_file_24_.get_data());

    for(line_t l : lines_t_) {
        draw_line(data_save, l);
    }
    
    // 将数据写入文件
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    file_name += "_hough_transform";
    std::string save_file_path = file_name + "." + file_ext;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream file(save_file_path, std::ios::binary);
    if (!file) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }

    // 将文件位图信息头和位图文件头写入磁盘
    BITMAPFILEHEADER file_header = bmp_file_24_.get_file_header();
    BITMAPINFOHEADER info_header = bmp_file_24_.get_info_header();
    file.write(reinterpret_cast<const char*>(&file_header), FILEHEADER_SIZE);
    file.write(reinterpret_cast<const char*>(&info_header), INFOHEDER_SIZE);

    // 将位图板写入磁盘

    // 将位图数据写入磁盘
    file.write(reinterpret_cast<const char*>(data_save), sizeof(uint8_t)*(line_bytes_24 * height));

    // 释放资源
    delete[] data;
    delete[] data_save;

    // 关闭文件资源
    file.close();
    
}

void hough_transformation::convert_8_to_24() {
    // 图像基本信息
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();

    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;
    int32_t line_bytes_24 = (width * 24 / 8 + 3) / 4 * 4;

    //  内存中生成24位bmp文件
    bmp_file_24_.set_file_header(bmp_file_.get_file_header());
    bmp_file_24_.set_info_header(bmp_file_.get_info_header());

    // 修改位图文件头
    bmp_file_24_.set_bf_size(FILEHEADER_SIZE + INFOHEDER_SIZE + line_bytes_24 * height);
    bmp_file_24_.set_bf_off_bits(FILEHEADER_SIZE + INFOHEDER_SIZE );

    // 修改位图信息头
    bmp_file_24_.set_bit_count(24);
    bmp_file_24_.set_size_image(line_bytes_24 * height);

    // 生成24位数据
    uint8_t* data_24 = new uint8_t[line_bytes_24 * height];

    for(int i = 0; i < height; ++i) {
        for(int j =  0; j < width; ++j) {
            uint8_t d = *(data + i * line_bytes + j);
            *(data_24 + i * line_bytes_24 + 3 * j) = d;
            *(data_24 + i * line_bytes_24 + 3 * j + 1) = d;
            *(data_24 + i * line_bytes_24 + 3 * j + 2) = d;
        }
    }

    // 保存data_24
    bmp_file_24_.set_data(reinterpret_cast<char*>(data_24));


}

void hough_transformation::draw_line(uint8_t* data, line_t line){
    std::random_device rd;  // 用于获取随机种子
    std::mt19937 gen(rd());  // 使用 Mersenne Twister 引擎进行随机数生成
    std::uniform_int_distribution<> dis(1, 3);  // 生成1到3之间的均匀分布整数

    int random = dis(gen);  // 生成随机数

    int agl = line.theta;
    int p = line.p;

    int32_t line_bytes_24 = (width_ * 24 / 8 + 3) / 4 * 4;

    for(int x = 0; x < height_ ; x++) {
        int y = -(cos(agl * M_PI / 180.0) / sin(agl * M_PI / 180.0)) * x + p / (sin(agl * M_PI / 180.0));
        if(y < 0 || y >= width_){continue;}

        switch(random) {
            case 1:{
                *(data + line_bytes_24 * x + 3 * y) = 255;
                *(data + line_bytes_24 * x + 3 * y + 1) = 0;
                *(data + line_bytes_24 * x + 3 * y + 2) = 0;
                break;
            }
            case 2:{
                *(data + line_bytes_24 * x + 3 * y) = 0;
                *(data + line_bytes_24 * x + 3 * y + 1) = 255;
                *(data + line_bytes_24 * x + 3 * y + 2) = 0;
                break;
            }
            case 3:{
                *(data + line_bytes_24 * x + 3 * y) = 0;
                *(data + line_bytes_24 * x + 3 * y + 1) = 0;
                *(data + line_bytes_24 * x + 3 * y + 2) = 255;
                break;
            }
        }
    }

}