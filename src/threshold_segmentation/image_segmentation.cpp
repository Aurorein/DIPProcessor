#include "image_segmentation.h"

void image_segmentation::read_file(){
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

uint32_t image_segmentation::find_max_in_array(uint32_t arr[], uint32_t size_t){
    uint32_t max = 0;
    for(uint32_t i = 0 ; i < size_t ; i++){
        if(arr[i] > max){
            max = arr[i];
        }
    }
    return max;
}

void image_segmentation::output_segmentation_res() {
    save_file_.set_file_header(bmp_file_.get_file_header());
    save_file_.set_info_header(bmp_file_.get_info_header());

    histogram_file_.set_queue(bmp_file_.get_queue());
    // 获取bmp文件的头信息
    uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
    int32_t width = bmp_file_.get_width();
    int32_t height = bmp_file_.get_height();

    int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;

    // 保存图像分割后的数据数组
    uint8_t *data_segmentation = new uint8_t[line_bytes * height];

    for(int32_t i  = 0; i < height; ++i) {
        for(int32_t j = 0; j < width; ++j) {
            uint8_t tmp = *(data + j * line_bytes + i);
            *(data_segmentation + j * line_bytes + i) = (tmp > threshold_) ? 255 : 0;
        }
    }

    save_file_.set_data(reinterpret_cast<char *>(data_segmentation));

    // 根据给定阈值分割
    size_t pos1 = file_path_.find_last_of(".");
    if(pos1 == std::string::npos) {
        throw std::exception();
    }
    std::string file_name1 = file_path_.substr(0, pos1);
    std::string file_ext1 = file_path_.substr(pos1 + 1);

    file_name1 += get_type() + "_segmentation";
    std::string save_file_path1 = file_name1 + "." + file_ext1;

    // 将位图文件头和位图信息头保存到文件中
    std::ofstream file1(save_file_path1, std::ios::binary);
    if (!file1) {
        std::cout << "failed to create the BMP file to save." << std::endl;
        return;
    }

    // 将文件位图信息头和位图文件头写入磁盘
    BITMAPFILEHEADER file_header1 = save_file_.get_file_header();
    BITMAPINFOHEADER info_header1 = save_file_.get_info_header();
    file1.write(reinterpret_cast<const char*>(&file_header1), FILEHEADER_SIZE);
    file1.write(reinterpret_cast<const char*>(&info_header1), INFOHEDER_SIZE);

    // 将位图板写入磁盘
    file1.write(reinterpret_cast<const char*>(bmp_file_.get_queue()), sizeof(RGBQUAD)*(256));

    // 将位图数据写入磁盘
    file1.write(reinterpret_cast<const char*>(data_segmentation), sizeof(uint8_t)*(line_bytes * height));

    // 释放资源
    

    // 关闭文件资源
    file1.close();


    // 下面是生成直方图
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

    uint8_t* bmp_data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());

    // 声明一个长度为256的数组，用来统计每个灰度的频度
    uint32_t *frequen_array = new uint32_t[256]{0};

    // 对灰度图每一个位图数据byte，将其统计到灰度频度数组中

    for(uint32_t i = 0; i < height; i++){
        for(uint32_t j = 0 ;j < width; j++){
            uint32_t gray_data = *(bmp_data + i*line_bytes + j);
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

    for(uint32_t i = 0; i < 255; ++i) {
        if(i % 2 == 0){
            *(histogram_data + i* line_bytes_histogram + threshold_) = 255;
        }else{
            *(histogram_data + i* line_bytes_histogram + threshold_) = 0;
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

    file_name += get_type() + "_segmentation_histogram";
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
    delete[] data_segmentation;

    // 关闭文件资源
    file.close();
}