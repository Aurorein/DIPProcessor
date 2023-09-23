#include "convolution_mask.h"

class average_filtering_convolution : public convolution_mask{
public:
    average_filtering_convolution(uint8_t mask_height, uint8_t mask_width) {
        mask_height_ = mask_height;
        mask_width_ = mask_width;
        mask_kernel_ = new uint8_t[mask_height_ * mask_width_];
        // 均值滤波，所有的值都是一样的
        for(int i = 0; i < mask_height_*mask_width_; ++i){
            *(mask_kernel_ + i) = 1;
        }
        sum_coeff_ = mask_height_ * mask_width_;
    }

    ~average_filtering_convolution(){
        delete[] mask_kernel_;
    }


};