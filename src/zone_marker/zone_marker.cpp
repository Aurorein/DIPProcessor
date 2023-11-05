#include "zone_marker.h"

void zone_marker::read_file() {
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

void zone_marker::convert_8_to_24(){
    // 图像基本信息
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t width = bmp_file_.get_width();
    width_ = width;
    int32_t height = bmp_file_.get_height();
    height_ = height;

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

void zone_marker::execute_zone_mark(){
    // vis_变量初始化
    vis_ = new short*[height_];
    for(int i = 0 ; i < height_; ++i) {
        vis_[i] = new short[width_]{0};
    }

    int32_t line_bytes = (width_ * 8 / 8 + 3) / 4 * 4;
    int32_t line_bytes_24 = (width_ * 24 / 8 + 3) / 4 * 4;
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());

    // 统计vis_
    for(int i = 0 ; i < height_; ++i) {
        for(int j  = 0 ; j < width_; ++j) {
            
            if(vis_[i][j] < 1){
                dfs(data, i , j);
                dye();
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

    file_name += "_zone_marker";
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

    uint8_t* data_24 = reinterpret_cast<uint8_t*>(bmp_file_24_.get_data());

    // 将位图数据写入磁盘
    file.write(reinterpret_cast<const char*>(data_24), sizeof(uint8_t)*(line_bytes_24 * height_));

    // 释放资源
    // delete[] data;

    // 关闭文件资源
    file.close();

    // 释放vis_变量的内存
    for(int i = 0 ; i < height_; ++i){
        delete[] vis_[i];
    }
}

void zone_marker::dfs(uint8_t* data, int x, int y) {
    int32_t line_bytes = (width_ * 8 / 8 + 3) / 4 * 4;

    int coor[5] = {0, -1, 0, 1, 0};

    vis_[x][y] = 1;

    for(int k = 0; k < 4; ++ k){
        int xx = x + coor[k];
        int yy = y + coor[k + 1];

        if( xx < 0 || xx >= height_ || yy < 0 || yy >= width_ || vis_[xx][yy] > 0 || *(data + x * line_bytes + y) != *(data + xx * line_bytes + yy)){
            continue;
        }

        dfs(data, xx, yy);
    }
    

}

void zone_marker::dye(){
    std::random_device rd;  // 用于获取随机种子
    std::mt19937 gen(rd());  // 使用 Mersenne Twister 引擎进行随机数生成
    std::uniform_int_distribution<> dis(1, 6);  // 生成1到3之间的均匀分布整数

    int random = dis(gen);  // 生成随机数

    // 获得24位图的数据
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_24_.get_data());

    int32_t line_bytes_24 = (width_ * 24 / 8 + 3) / 4 * 4;

    for(int i = 0 ; i < height_; ++i) {
        for(int j = 0; j < width_; ++j) {
            if(vis_[i][j] == 1){
                switch(random) {
                    case 1:{
                        *(data + i * line_bytes_24 + 3 * j) = 255;
                        *(data + i * line_bytes_24 + 3 * j + 1) = 0;
                        *(data + i * line_bytes_24 + 3 * j + 2) = 0;
                        break;
                    }
                    case 2:{
                        *(data + i * line_bytes_24 + 3 * j) = 0;
                        *(data + i * line_bytes_24 + 3 * j + 1) = 255;
                        *(data + i * line_bytes_24 + 3 * j + 2) = 0;
                        break;
                    }
                    case 3:{
                        *(data + i * line_bytes_24 + 3 * j) = 0;
                        *(data + i * line_bytes_24 + 3 * j + 1) = 0;
                        *(data + i * line_bytes_24 + 3 * j + 2) = 255;
                        break;
                    }
                    case 4:{
                        *(data + i * line_bytes_24 + 3 * j) = 255;
                        *(data + i * line_bytes_24 + 3 * j + 1) = 255;
                        *(data + i * line_bytes_24 + 3 * j + 2) = 0;
                        break;
                    }
                    case 5:{
                        *(data + i * line_bytes_24 + 3 * j) = 255;
                        *(data + i * line_bytes_24 + 3 * j + 1) = 0;
                        *(data + i * line_bytes_24 + 3 * j + 2) = 255;
                        break;
                    }
                    case 6:{
                        *(data + i * line_bytes_24 + 3 * j) = 0;
                        *(data + i * line_bytes_24 + 3 * j + 1) = 255;
                        *(data + i * line_bytes_24 + 3 * j + 2) = 255;
                        break;
                    }
                    
                    
                }
                vis_[i][j] = 2;
            }
        }
    }
}
