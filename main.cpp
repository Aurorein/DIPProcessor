#include<iostream>
#include<cstdlib>
#include "image_process.h"
#include<memory>

auto image_process_ = std::make_unique<image_process>();


int main(int argc, char *argv[]){
    
    if(argc <= 1){
        std::cout<<"missing image path arg!"<<std::endl;
        return 1;
    }
    std::string image_path = argv[1];

    // 处理bmp格式文件
    image_process *image_process_ = new image_process();
    image_process_ ->read_file(image_path);
    


    return 0;
}