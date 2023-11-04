#pragma once

#include<iostream>


class edge_detection_operator {
public:
    int8_t* operator_kernel_X_;    // GX算子核的数据
    int8_t* operator_kernel_Y_;    // GY算子核的数据    
    uint32_t sum_coeff_;          // 算子系数和

    virtual uint8_t operator_compute(uint8_t* data){
        int GX;
        int GY;

        int32_t res = 0;
        for(int i = 0;i < 3 ;++i){
            for(int j = 0 ; j< 3 ;++j){
                res += *(data + i * 3 + j )* *(operator_kernel_X_ + i * 3 + j);         
            }
        }
        // 如果res溢出（大于255或者小于0），做统一化处理
        if(res > 255){
            res = 255;
        }

        if(res < 0){
            res = 0;
        }

        GX = uint8_t(res);

        for(int i = 0;i < 3 ;++i){
            for(int j = 0 ; j< 3 ;++j){
                res += *(data + i * 3 + j )* *(operator_kernel_Y_ + i * 3 + j);         
            }
        }

        // 如果res溢出（大于255或者小于0），做统一化处理
        if(res > 255){
            res = 255;
        }

        if(res < 0){
            res = 0;
        }

        GY = uint8_t(res);

        res = abs(GX) + abs(GY);

        if(res > 255){
            res = 255;
        }

        return uint8_t(res);
    }



};
