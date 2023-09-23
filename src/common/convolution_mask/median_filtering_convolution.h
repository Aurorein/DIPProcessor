#include "convolution_mask.h"
#include<vector>
#include<algorithm>

class median_filtering_convolution : public convolution_mask{
public:
    median_filtering_convolution(uint8_t mask_height, uint8_t mask_width){
        mask_height_ = mask_height;
        mask_width_ = mask_width;  
    }

    uint8_t convolution_compute(uint8_t* data){
        uint32_t size_matrix = mask_height_ * mask_width_;
        std::vector<uint8_t> matrix_arr;
        for(uint32_t i = 0; i < mask_height_; ++i){
            for(uint32_t j = 0; j < mask_width_; ++j){
                uint8_t val = *(data + i * mask_height_ + j);
                matrix_arr.emplace_back(val);
            }
        }
        // 寻找matrix数组中的中位数
        std::sort(matrix_arr.begin(), matrix_arr.end());

        int median_index = size_matrix / 2;
        return matrix_arr[median_index];
    }


private:

};