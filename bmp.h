#include<iostream>

#pragma pack(push)
#pragma pack(2)
#pragma pack(pop)


/**
 * @brief 位图文件头
 * 
 */
struct BITMAPFILEHEADER{
    uint16_t    b_type_;  
    uint32_t   bf_size_;
    uint16_t    bf_reserved1_;
    uint16_t    bf_reserved2_;
    uint32_t   bf_off_bits_;
};

/**
 * @brief 位图信息头
 * 
 */
struct BITMAPINFOHEADER{
    uint32_t   bi_size_;
    int32_t      bi_width_;
    int32_t      bi_height_;
    uint16_t    bi_planes_;
    uint16_t    bi_bit_count_;
    uint32_t   bi_compression_;
    uint32_t   bi_size_image_;
    int32_t      bi_x_pels_per_meter_;
    int32_t      bi_y_pels_per_meter_;
    uint32_t   bi_clr_used_;
    uint32_t   bi_clr_important_;
};

/**
 * @brief 调色板
 * 
 */
struct RGBQUAD{
    uint16_t    rgb_blue_;
    uint16_t    rgb_green_;
    uint16_t    rgb_red_;
    uint16_t    rgb_reserved_;
};

class bmp{
private:
    BITMAPFILEHEADER file_header_;
    BITMAPINFOHEADER info_header_;
    RGBQUAD* queue_;
    char* data_;
public:



    void serialize_file_header(){}
    void deserialize_file_header(char *src){
        int offset = 0;
        file_header_.b_type_ = *reinterpret_cast<const uint16_t*>(src);
        offset += sizeof(uint16_t);
        file_header_.bf_off_bits_ = *reinterpret_cast<const uint32_t*>(src + offset);
        offset += sizeof(uint32_t);
        file_header_.bf_reserved1_ = *reinterpret_cast<const uint16_t*>(src + offset);
        offset += sizeof(uint16_t);
        file_header_.bf_reserved2_ = *reinterpret_cast<const uint16_t*>(src + offset);
        offset += sizeof(uint16_t);
        file_header_.bf_size_ = *reinterpret_cast<const uint32_t*>(src + offset);
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


};



