#include "region_spliting.h"

void region_spliting::read_file(){
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


void region_spliting::execute_spliting() {
    // 获取bmp文件的头信息
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();

    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;

    uint32_t size_image = bmp_file_.get_size_image();

    // 二维数组记录是否标记过
    bool** vis = new bool*[height];
    for(int i = 0 ; i < height; i++) {
        vis[i] = new bool[width];

    }

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            vis[i][j] = false;
        }
    }

    // 开始区域分割
    dfs(vis, data, 0, height - 1, 0, width - 1);

    // 生成分割轨迹
    for(int i = 0 ; i < height; ++i ){
        for(int j = 0; j < width; ++j) {
            if(vis[i][j]){
                *(data + i * line_bytes + j)  = 0;
            }
             
        }
    }

    // 写入文件
    size_t pos = file_path_.find_last_of(".");
    if(pos == std::string::npos) {
        throw std::exception();
    }
    std::string file_name = file_path_.substr(0, pos);
    std::string file_ext = file_path_.substr(pos + 1);

    file_name += "_region_spliting";
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
    file.write(reinterpret_cast<const char*>(data), sizeof(uint8_t)*size_image);

    // 释放资源
    delete[] data;

    // 关闭文件资源
    file.close();

}

void region_spliting::dfs(bool** vis, uint8_t* data, int i1, int i2, int j1, int j2){
    if(i2 - i1 < 2){
        return;
    }
    if(j2 - j1 < 2){
        return;
    }

    int midi = (i1 + i2)/2;
    int midj = (j1 + j2)/2;

    for(int i = i1; i <= i2; ++i) {
        vis[i][midj] = true;
    }

    for(int j = j1; j <= j2; ++j) {
        vis[midi][j] = true;
    }

    if(!match_distance(data, i1, midi, j1, midj)){
        dfs(vis, data, i1, midi, j1, midj);
    }
    if(!match_distance(data, midi, i2, j1, midj)){
        dfs(vis, data, midi, i2, j1, midj);
    }
    if(!match_distance(data, i1, midi, midj, j2)){
        dfs(vis, data, i1, midi, midj, j2);
    }
    if(!match_distance(data, midi, i2, midj, j2)){
        dfs(vis, data, midi, i2, midj, j2);
    }

    
}

bool region_spliting::match_distance(uint8_t* data, int i1, int i2, int j1, int j2) {
    int* coor = new int[5]{0, 1, 0, -1, 0};

    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();

    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;

    for(int i = i1 + 1; i < i2; i++){
        for(int j  = j1 + 1; j < j2; j++) {
            for(int k = 0; k < 4; ++k){
                int tmp = *(data + i * line_bytes + j);
                if(*(data + (i + coor[k]) * line_bytes + (j + coor[k + 1])) - tmp >= 15 ){
                    return false;
                }
            }
            
        }
    }

    return true;
}