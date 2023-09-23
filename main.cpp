#include<iostream>
#include<cstdlib>
#include "./src/image_process/image_process.h"
#include "./src/image_histogram/image_histogram.h"
#include "./src/spatial_domain_filtering/spatial_domain_filtering.h"
#include "./src/spatial_domain_filtering/average_filtering.h"
#include "./src/spatial_domain_filtering/median_filtering.h"
#include<memory>




int main(int argc, char *argv[]){
    
    if(argc <= 1){
        std::cout<<"missing image path arg!"<<std::endl;
        return 1;
    }
    std::string image_path = argv[1];

    /**
     * 图像变换处理
     */
    // auto image_process_ = std::make_unique<image_process>(image_path);

    // image_process_ ->read_file();
    
    // image_process_->convert_to_gray();
    // image_process_->inverse_gray_color();

    // image_process_->channel_split();

    /**
     * 图像直方图
     */
    // auto image_histogram_ = std::make_unique<image_histogram>(image_path);

    // image_histogram_->read_file();

    // image_histogram_->get_histogram_file();

    // image_histogram_->get_histogram_equaled();

    /**
     * 滤波器处理
     */

    // 1. 均值滤波
    auto spatial_domain_filtering_ = std::make_unique<average_filtering>(image_path, 1);

    spatial_domain_filtering_->read_file();

    spatial_domain_filtering_->do_filtering(spatial_domain_filtering_->get_border()); 

    // 2. 中值滤波
    auto median_domain_filtering_ = std::make_unique<median_filtering>(image_path, 1);

    median_domain_filtering_->read_file();

    median_domain_filtering_->do_filtering(median_domain_filtering_->get_border());

    return 0;
}