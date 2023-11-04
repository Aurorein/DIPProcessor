#pragma once

#include<iostream>
#include "coordinate.h"

class transformational_matrix{
public:
    transformational_matrix(){}

    void set_data(float *m_data){m_data_ = m_data;}

    virtual coordinate compute(coordinate point_){
        coordinate point_transform;

        for(uint32_t i = 0; i < 3; ++i){
            for(uint32_t j = 0; j < 3 ; ++j ){
                point_transform.x_ = point_.x_ * m_data_[0] + point_.y_ * m_data_[1] + point_.z_ * m_data_[2];
                point_transform.y_ = point_.x_ * m_data_[3] + point_.y_ * m_data_[4] + point_.z_ * m_data_[5];
                point_transform.z_ = point_.x_ * m_data_[6] + point_.y_ * m_data_[7] + point_.z_ * m_data_[8]; 
            }
        }

        return point_transform;
    };
private:
    // uint8_t m_height_;    // 矩阵的高度
    // uint8_t m_width_;     // 矩阵的宽度
    float *m_data_;      // 矩阵的数据

};
