#pragma once

#include<iostream>

// 位图文件头使用2字节对齐
#pragma pack(push,2)


/**
 * @brief 位图文件头
 * 
 */
struct BITMAPFILEHEADER{
    uint16_t    bf_type_;           // 文件类型，必须是"BM"
    uint32_t   bf_size_;           // 文件大小，必须包含头文件的大小
    uint16_t    bf_reserved1_;     // 保留字
    uint16_t    bf_reserved2_;     // 保留字
    uint32_t   bf_off_bits_;       // 从文件头到实际位图数据的偏移字节

    BITMAPFILEHEADER& operator=(const BITMAPFILEHEADER& other){
        if(this != &other){
            bf_type_ = other.bf_type_;
            bf_size_ = other.bf_size_;
            bf_reserved1_ = other.bf_reserved1_;
            bf_reserved2_ = other.bf_reserved2_;
            bf_off_bits_ = other.bf_off_bits_;
        }
        return *this;
    }
};

// 位图信息头取消2字节对齐
#pragma pack(pop)

/**
 * @brief 位图信息头
 * 
 */
struct BITMAPINFOHEADER{
    uint32_t   bi_size_;                // 该结构的长度，为40
    int32_t      bi_width_;             // 图像宽度
    int32_t      bi_height_;            // 图像高度
    uint16_t    bi_planes_;             // 位平面数，必须为1
    uint16_t    bi_bit_count_;          // 颜色位数
    uint32_t   bi_compression_;         // 是否压缩
    uint32_t   bi_size_image_;          // 实际位图数据占用的字节数
    int32_t      bi_x_pels_per_meter_;  // 目标设备的水平分辨率
    int32_t      bi_y_pels_per_meter_;  // 目标设备的垂直分辨率
    uint32_t   bi_clr_used_;            // 实际使用的颜色数
    uint32_t   bi_clr_important_;       // 图像中重要的颜色数

    BITMAPINFOHEADER& operator=(const BITMAPINFOHEADER& other){
        if(this != &other){
            bi_size_ = other.bi_size_;
            bi_width_ = other.bi_width_;
            bi_height_ = other.bi_height_;
            bi_planes_ = other.bi_planes_;
            bi_bit_count_ = other.bi_bit_count_;
            bi_compression_ = other.bi_compression_;
            bi_size_image_ = other.bi_size_image_;
            bi_x_pels_per_meter_ = other.bi_x_pels_per_meter_;
            bi_y_pels_per_meter_ = other.bi_y_pels_per_meter_;
            bi_clr_used_ = other.bi_clr_used_;
            bi_clr_important_ = other.bi_clr_important_;
        }
        return *this;
    }
};

/**
 * @brief 调色板
 * 
 */
struct RGBQUAD{
    uint8_t    rgb_blue_;           // 该颜色的蓝色分量
    uint8_t    rgb_green_;          // 该颜色的绿色分量
    uint8_t    rgb_red_;            // 该颜色的红色分量
    uint8_t    rgb_reserved_;       // 保留值
};

const int FILEHEADER_SIZE = sizeof(BITMAPFILEHEADER);
const int INFOHEDER_SIZE = sizeof(BITMAPINFOHEADER);

class bmp{
private:
    BITMAPFILEHEADER file_header_;
    BITMAPINFOHEADER info_header_;
    RGBQUAD* queue_;
    char* data_;
public:
    ~bmp(){

        // 防止内存泄漏

        // delete data_;
        // delete queue_;
        
    }

    void serialize_file_header(){}
    void deserialize_file_header(char *src){
        int offset = 0;
        file_header_.bf_type_ = *reinterpret_cast<const uint16_t*>(src);
        offset += sizeof(uint16_t);
        file_header_.bf_size_ = *reinterpret_cast<const uint32_t*>(src + offset);
        offset += sizeof(uint32_t);
        file_header_.bf_reserved1_ = *reinterpret_cast<const uint16_t*>(src + offset);
        offset += sizeof(uint16_t);
        file_header_.bf_reserved2_ = *reinterpret_cast<const uint16_t*>(src + offset);
        offset += sizeof(uint16_t);
        file_header_.bf_off_bits_ = *reinterpret_cast<const uint32_t*>(src + offset);
    }

    void serialize_info_header(){}
    void deserialize_info_header(char *src){
        int offset = 0;
        info_header_.bi_size_ = *reinterpret_cast<const uint32_t*>(src);
        offset += sizeof(uint32_t);
        info_header_.bi_width_ = *reinterpret_cast<const int32_t*>(src + offset);
        offset += sizeof(int32_t);
        info_header_.bi_height_ = *reinterpret_cast<const int32_t*>(src + offset);
        offset += sizeof(int32_t);
        info_header_.bi_planes_ = *reinterpret_cast<const uint16_t*>(src + offset);
        offset += sizeof(uint16_t);
        info_header_.bi_bit_count_ = *reinterpret_cast<const uint16_t*>(src + offset);
        offset += sizeof(uint16_t);
        info_header_.bi_compression_ = *reinterpret_cast<const uint32_t*>(src + offset);
        offset += sizeof(uint32_t);
        info_header_.bi_size_image_ = *reinterpret_cast<const uint32_t*>(src + offset);
        offset += sizeof(uint32_t);
        info_header_.bi_x_pels_per_meter_ = *reinterpret_cast<const int32_t*>(src + offset);
        offset += sizeof(int32_t);
        info_header_.bi_y_pels_per_meter_ = *reinterpret_cast<const int32_t*>(src + offset);
        offset += sizeof(int32_t);
        info_header_.bi_clr_used_ = *reinterpret_cast<const uint32_t*>(src + offset);
        offset += sizeof(uint32_t);
        info_header_.bi_clr_important_ = *reinterpret_cast<const uint32_t*>(src + offset);
    }

    uint16_t get_bit_count(){return info_header_.bi_bit_count_;}

    uint32_t get_size_image(){return info_header_.bi_size_image_;}

    char* get_data(){return data_;}

    int32_t get_width(){return info_header_.bi_width_;}

    int32_t get_height(){return info_header_.bi_height_;}

    BITMAPFILEHEADER get_file_header(){return file_header_;}

    BITMAPINFOHEADER get_info_header(){return info_header_;}

    uint32_t get_bf_off_bits(){return file_header_.bf_off_bits_;}

    RGBQUAD* get_queue(){return queue_;}



    void set_bit_count(uint16_t bi_bit_count){info_header_.bi_bit_count_ = bi_bit_count;}

    void set_size_image(uint32_t bi_size_image){info_header_.bi_size_image_ = bi_size_image;}

    void set_queue(RGBQUAD *queue){queue_ = queue;}

    void set_clr_used(uint32_t bi_clr_used){info_header_.bi_clr_used_ = bi_clr_used;}

    void set_data(char *data){data_ = data;}

    void set_file_header(BITMAPFILEHEADER file_header){file_header_ = file_header;}

    void set_info_header(BITMAPINFOHEADER info_header){info_header_ = info_header;}

    void set_bf_size(uint32_t bf_size){file_header_.bf_size_ = bf_size;}

    void set_bf_off_bits(uint32_t bf_offset_bits){file_header_.bf_off_bits_ = bf_offset_bits;}

    void set_height(int32_t bi_height){info_header_.bi_height_ = bi_height;}

    void set_width(int32_t bi_width){info_header_.bi_width_ = bi_width;}
};



