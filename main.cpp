#include<iostream>
#include<cstdlib>
#include "./src/image_process/image_process.h"
#include "./src/image_histogram/image_histogram.h"
#include "./src/spatial_domain_filtering/spatial_domain_filtering.h"
#include "./src/spatial_domain_filtering/average_filtering.h"
#include "./src/spatial_domain_filtering/median_filtering.h"
#include "./src/image_transformation/translation_transformation.h"
#include "./src/image_transformation/scaling_transformation.h"
#include "./src/image_transformation/mirror_transformation.h"
#include "./src/image_transformation/rotate_transformation.h"
#include "./src/threshold_segmentation/image_segmentation.h"
#include "./src/threshold_segmentation/threshold_t.h"
#include "./src/threshold_segmentation/iterative_threshold.h"
#include "./src/threshold_segmentation/ostu.h"
#include "./src/region_segmentation/region_growing.h"
#include "./src/region_segmentation/region_spliting.h"
#include "./src/edge_detection/prewitt_detection.h"
#include "./src/edge_detection/sobel_detection.h"
#include "./src/edge_detection/log_detection.h"
#include "./src/hough_transform/hough_transform.h"
#include "./src/zone_marker/zone_marker.h"
#include "./src/contour_extraction/contour_extraction.h"

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
    // auto spatial_domain_filtering_ = std::make_unique<average_filtering>(image_path, 3);

    // spatial_domain_filtering_->read_file();

    // spatial_domain_filtering_->do_filtering(spatial_domain_filtering_->get_border()); 

    // // 2. 中值滤波
    // auto median_domain_filtering_ = std::make_unique<median_filtering>(image_path, 3);

    // median_domain_filtering_->read_file();

    // median_domain_filtering_->do_filtering(median_domain_filtering_->get_border());

    /**
     * 图像变换
     */
    // 1. 平移变换
    // auto translation_transformation_ = std::make_unique<translation_transfotmation>(image_path, 10, 10);

    // translation_transformation_->read_file();

    // translation_transformation_->transformation();
    // 2. 缩放变换
    // auto scaling_transformation_ = std::make_unique<scaling_transformation>(image_path, 0.2, 0.2);

    // scaling_transformation_->read_file();

    // scaling_transformation_->transformation();
    // 3. 镜像变换
    // auto mirror_transformation_ = std::make_unique<mirror_transfotmation>(image_path, 203);

    // mirror_transformation_->read_file();

    // mirror_transformation_->transformation();
    // 4. 旋转图像
    // auto rotate_transformation_ = std::make_unique<rotate_transformation>(image_path, 30.0);

    // rotate_transformation_->read_file();

    // rotate_transformation_->transformation();

    /**
     * 图像分割
     */
    // 给定阈值T
    // auto t_threshold_ = std::make_unique<t_threshold_segmentation>(image_path, 100);

    // t_threshold_->read_file();

    // t_threshold_->output_segmentation_res();

    // 迭代阈值法
    // auto iterative_threshold_ = std::make_unique<iterative_threshold_segmentation>(image_path);

    // iterative_threshold_->read_file();

    // iterative_threshold_->get_threshold();

    // iterative_threshold_->output_segmentation_res();

    // ostu阈值法
    // auto ostu_threshold_ = std::make_unique<ostu_threshold_segmentation>(image_path);

    // ostu_threshold_->read_file();

    // ostu_threshold_->get_threshold();

    // ostu_threshold_->output_segmentation_res();

    /**
     * 区域分割
     */
    // 区域增长
    // std::vector<int*> coors;

    // coors.emplace_back(new int[2]{100, 100}); 

    // coors.emplace_back(new int[2]{200, 20});

    // auto region_growing_ = std::make_unique<region_growing>(image_path, coors);

    // region_growing_->read_file();

    // region_growing_->excute_growing();

    // 区域增强
    // auto region_spliting_ = std::make_unique<region_spliting>(image_path);

    // region_spliting_->read_file();

    // region_spliting_->execute_spliting();

    /**
     * 边缘检测--prewitt算子
     */
    // auto prewitt_detection_ = std::make_unique<prewitt_detection>(image_path);

    // prewitt_detection_->read_file();

    // prewitt_detection_->execute_detection();

    /**
     * 边缘检测--sobel算子
     */
    // auto sobel_detection_ = std::make_unique<sobel_detection>(image_path);

    // sobel_detection_->read_file();

    // sobel_detection_->execute_detection();

    /**
     * 边缘检测--log算子
     */
    // auto log_detection_ = std::make_unique<log_detection>(image_path);

    // log_detection_->read_file();

    // log_detection_->execute_detection();

    /**
     * hough变换
     */
    // auto hough_transform_ = std::make_unique<hough_transformation>(image_path);

    // hough_transform_->read_file();

    // hough_transform_->convert_8_to_24();

    // hough_transform_->execute_transformation();

    /**
     * 区域标记
     */
    // auto zone_marker_ = std::make_unique<zone_marker>(image_path);

    // zone_marker_->read_file();

    // zone_marker_->convert_8_to_24();

    // zone_marker_->execute_zone_mark();
    
    /**
     * 轮廓提取
     */
    // auto contour_extraction_ = std::make_unique<contour_extraction>(image_path);

    // contour_extraction_->read_file();

    // contour_extraction_->execute_contour_extract();

    return 0;
}