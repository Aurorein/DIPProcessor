#pragma once

#include<iostream>

class convolution_mask{
public:
    uint8_t mask_width_;          // 卷积核的宽度
    uint8_t mask_height_;         // 卷积核的高度
    uint8_t* mask_kernel_;        // 卷积核的数据
    uint32_t sum_coeff_;          // 卷积系数和

    virtual uint8_t convolution_compute(uint8_t* data){
        int32_t res = 0;
        for(int i = 0;i < mask_height_ ;++i){
            for(int j = 0 ; j< mask_width_ ;++j){
                res += *(data + i * mask_width_ + j )* *(mask_kernel_ + i * mask_width_ + j);         
            }
        }
        res = res/ sum_coeff_;
        // 如果res溢出（大于255或者小于0），做统一化处理
        if(res > 255){
            res = 255;
        }

        if(res < 0){
            res = 0;
        }

        return uint8_t(res);
    }

    uint32_t get_sum_coeff(){return sum_coeff_;}
private:
};