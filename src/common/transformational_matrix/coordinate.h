#pragma once

class coordinate{
public:
    coordinate() : x_(0), y_(0), z_(0){} 
    coordinate(float x, float y, float z) : x_(x), y_(y), z_(z){}

    float x_;    // x坐标
    float y_;    // y坐标
    float z_;    // z坐标，在几何变换中为1
};