#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<map>
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


std::vector<std::string> split(std::string &s, std::string &separator)
{
    int t = 1, pre = 0;
    std::vector<std::string> res;
    while (t != -1)
    {
        t = s.find(separator, pre);
        if (t != -1)
            res.push_back(s.substr(pre, t - pre));
        else
            break;
        pre = t + separator.size();
    }
    if (pre < s.size())
        res.push_back(s.substr(pre));
    return res;
};


int main(int argc, char *argv[]){
    
    // if(argc <= 1){
    //     std::cout<<"missing image path arg!"<<std::endl;
    //     return 1;
    // }
    // std::string image_path = argv[1];

    std::cout<<"digital image process program     "<<std::endl;
    // std::cout<<"   1. file process    "<<std::endl;
    // std::cout<<"   2. histogram processing     "<<std::endl;
    // std::cout<<"   3. spatial domain filtering     "<<std::endl;
    // std::cout<<"   4. image transformation     "<<std::endl;
    // std::cout<<"   5. threshold segmentation     "<<std::endl;
    // std::cout<<"   6. segmentation based on region     "<<std::endl;
    // std::cout<<"   7. edge detection     "<<std::endl;
    // std::cout<<"   8. hough transform     "<<std::endl;
    // std::cout<<"   9. zone marker     "<<std::endl;
    // std::cout<<"   10. contour extraction     "<<std::endl;
    std::cout<<"   input A single process mode"<<std::endl;
    std::cout<<"   input B batching process mode"<<std::endl;
    std::cout<<"input your mode:";

    char a;
    while(std::cin >> a){
        if(a == ' '){
            break;
        }
        if(a == 'A'){
            system("clear");
            std::cin.sync();
            std::cout<<"   1. file process    "<<std::endl;
            std::cout<<"   2. histogram processing     "<<std::endl;
            std::cout<<"   3. spatial domain filtering     "<<std::endl;
            std::cout<<"   4. image transformation     "<<std::endl;
            std::cout<<"   5. threshold segmentation     "<<std::endl;
            std::cout<<"   6. segmentation based on region     "<<std::endl;
            std::cout<<"   7. edge detection     "<<std::endl;
            std::cout<<"   8. hough transform     "<<std::endl;
            std::cout<<"   9. zone marker     "<<std::endl;
            std::cout<<"   10. contour extraction     "<<std::endl;
            std::cout<<"input the number before the function:";

            int num;
            std::cin>>num;
            
            std::cin.sync();
            std::cout<<"input the image file path:";
            std::string image_path;
            std::cin>>image_path;
            // sleep(1000);
            if(num > 10 || num < 0){
                system("cls");
                std::cerr<<"input number error!"<<std::endl;
                exit(0);
            }
            switch(num) {
                case 1:{
                    auto image_process_ = std::make_unique<image_process>(image_path);

                    image_process_ ->read_file();
                    
                    image_process_->convert_to_gray();

                    image_process_->channel_split();

                    size_t pos = image_path.find_last_of(".");
                    if(pos == std::string::npos) {
                        throw std::exception();
                    }
                    std::string file_name = image_path.substr(0, pos);
                    std::string file_ext = image_path.substr(pos + 1);

                    file_name += "_gray";
                    std::string new_file_path = file_name + "." + file_ext;
                    image_process_->set_image_file(new_file_path);

                    image_process_->read_file();

                    image_process_->inverse_gray_color();

                    std::cout<<"file process completed!"<<std::endl;
                    break;
                }
                case 2:{
                    auto image_histogram_ = std::make_unique<image_histogram>(image_path);

                    image_histogram_->read_file();

                    image_histogram_->get_histogram_file();

                    image_histogram_->get_histogram_equaled();

                    std::cout<<"histogram processing completed!"<<std::endl;
                    break;
                }
                case 3:{
                    // 1. 均值滤波
                    auto spatial_domain_filtering_ = std::make_unique<average_filtering>(image_path, 3);

                    spatial_domain_filtering_->read_file();

                    spatial_domain_filtering_->do_filtering(spatial_domain_filtering_->get_border()); 

                    // // 2. 中值滤波
                    auto median_domain_filtering_ = std::make_unique<median_filtering>(image_path, 3);

                    median_domain_filtering_->read_file();

                    median_domain_filtering_->do_filtering(median_domain_filtering_->get_border());
                    std::cout<<"spatial domain filtering completed!"<<std::endl;
                    break;
                }
                case 4:{
                    // 1. 平移变换
                    auto translation_transformation_ = std::make_unique<translation_transfotmation>(image_path, 10, 10);

                    translation_transformation_->read_file();

                    translation_transformation_->transformation();
                    // 2. 缩放变换
                    auto scaling_transformation_ = std::make_unique<scaling_transformation>(image_path, 0.2, 0.2);

                    scaling_transformation_->read_file();

                    scaling_transformation_->transformation();
                    // 3. 镜像变换
                    auto mirror_transformation_ = std::make_unique<mirror_transfotmation>(image_path, 203);

                    mirror_transformation_->read_file();

                    mirror_transformation_->transformation();
                    // 4. 旋转图像
                    auto rotate_transformation_ = std::make_unique<rotate_transformation>(image_path, 30.0);

                    rotate_transformation_->read_file();

                    rotate_transformation_->transformation();

                    std::cout<<"image transformation completed!"<<std::endl;
                    break;
                }
                case 5:{
                    // 给定阈值T
                    auto t_threshold_ = std::make_unique<t_threshold_segmentation>(image_path, 100);

                    t_threshold_->read_file();

                    t_threshold_->output_segmentation_res();

                    // 迭代阈值法
                    auto iterative_threshold_ = std::make_unique<iterative_threshold_segmentation>(image_path);

                    iterative_threshold_->read_file();

                    iterative_threshold_->get_threshold();

                    iterative_threshold_->output_segmentation_res();

                    // ostu阈值法
                    auto ostu_threshold_ = std::make_unique<ostu_threshold_segmentation>(image_path);

                    ostu_threshold_->read_file();

                    ostu_threshold_->get_threshold();

                    ostu_threshold_->output_segmentation_res();

                    std::cout<<"threshold segmentation completed!"<<std::endl;
                    break;
                }
                case 6:{
                    // 区域增长
                    std::vector<int*> coors;

                    coors.emplace_back(new int[2]{100, 100}); 

                    coors.emplace_back(new int[2]{200, 20});

                    auto region_growing_ = std::make_unique<region_growing>(image_path, coors);

                    region_growing_->read_file();

                    region_growing_->excute_growing();

                    // 区域增强
                    auto region_spliting_ = std::make_unique<region_spliting>(image_path);

                    region_spliting_->read_file();

                    region_spliting_->execute_spliting();

                    std::cout<<"segmentation based on region completed!"<<std::endl;
                    break;
                }
                case 7:{
                    // prewitt算子
                    auto prewitt_detection_ = std::make_unique<prewitt_detection>(image_path);

                    prewitt_detection_->read_file();

                    prewitt_detection_->execute_detection();

                    // sobel算子
                    auto sobel_detection_ = std::make_unique<sobel_detection>(image_path);

                    sobel_detection_->read_file();

                    sobel_detection_->execute_detection();

                    // log算子
                    auto log_detection_ = std::make_unique<log_detection>(image_path);

                    log_detection_->read_file();

                    log_detection_->execute_detection();

                    std::cout<<"edge detection completed!"<<std::endl;
                    break;
                }
                case 8:{
                    auto hough_transform_ = std::make_unique<hough_transformation>(image_path);

                    hough_transform_->read_file();

                    hough_transform_->convert_8_to_24();

                    hough_transform_->execute_transformation();

                    std::cout<<"hough transform completed!"<<std::endl;
                    break;
                }
                case 9:{
                    
                    auto zone_marker_ = std::make_unique<zone_marker>(image_path);

                    zone_marker_->read_file();

                    zone_marker_->convert_8_to_24();

                    zone_marker_->execute_zone_mark();

                    std::cout<<"zone marker completed!"<<std::endl;
                    break;
                }
                case 10:{
                    auto contour_extraction_ = std::make_unique<contour_extraction>(image_path);

                    contour_extraction_->read_file();

                    contour_extraction_->execute_contour_extract();

                    std::cout<<"contour extraction completed!"<<std::endl;
                    break;
                }
            }
            return 0;
        }else if(a == 'B') {
            system("clear");
            std::cin.sync();
            std::cout<<"   1. file process    "<<std::endl;
            std::cout<<"   2. histogram processing     "<<std::endl;
            std::cout<<"   3. spatial domain filtering     "<<std::endl;
            std::cout<<"   4. image transformation     "<<std::endl;
            std::cout<<"   5. threshold segmentation     "<<std::endl;
            std::cout<<"   6. segmentation based on region     "<<std::endl;
            std::cout<<"   7. edge detection     "<<std::endl;
            std::cout<<"   8. hough transform     "<<std::endl;
            std::cout<<"   9. zone marker     "<<std::endl;
            std::cout<<"   10. contour extraction     "<<std::endl;
            std::cout<<"batching process use 'num'-'filepath','num'-'filepath' format:"<<std::endl;
            std::cout<<"default mode input default:"<<std::endl;
            std::cout<<"give your input:";

            std::string input_string;
            std::cin>>input_string;

            if(input_string == "default")
            {
                std::string image_path = "../img/rgb.bmp";
                auto image_process_ = std::make_unique<image_process>(image_path);

                image_process_ ->read_file();
                
                image_process_->convert_to_gray();

                image_process_->channel_split();

                size_t pos = image_path.find_last_of(".");
                if(pos == std::string::npos) {
                    throw std::exception();
                }
                std::string file_name = image_path.substr(0, pos);
                std::string file_ext = image_path.substr(pos + 1);

                file_name += "_gray";
                std::string new_file_path = file_name + "." + file_ext;
                image_process_->set_image_file(new_file_path);

                image_process_->read_file();

                image_process_->inverse_gray_color();

                std::cout<<"file process completed!"<<std::endl;

                image_path = "../img/dim.bmp";

                auto image_histogram_ = std::make_unique<image_histogram>(image_path);

                image_histogram_->read_file();

                image_histogram_->get_histogram_file();

                image_histogram_->get_histogram_equaled();

                std::cout<<"histogram processing completed!"<<std::endl;

                image_path = "../img/noise.bmp";

                // 1. 均值滤波
                auto spatial_domain_filtering_ = std::make_unique<average_filtering>(image_path, 3);

                spatial_domain_filtering_->read_file();

                spatial_domain_filtering_->do_filtering(spatial_domain_filtering_->get_border()); 

                // 2. 中值滤波
                auto median_domain_filtering_ = std::make_unique<median_filtering>(image_path, 3);

                median_domain_filtering_->read_file();

                median_domain_filtering_->do_filtering(median_domain_filtering_->get_border());
                std::cout<<"spatial domain filtering completed!"<<std::endl;

                image_path = "../img/dim.bmp";

                // 1. 平移变换
                auto translation_transformation_ = std::make_unique<translation_transfotmation>(image_path, 10, 10);

                translation_transformation_->read_file();

                translation_transformation_->transformation();
                // 2. 缩放变换
                auto scaling_transformation_ = std::make_unique<scaling_transformation>(image_path, 0.2, 0.2);

                scaling_transformation_->read_file();

                scaling_transformation_->transformation();
                // 3. 镜像变换
                auto mirror_transformation_ = std::make_unique<mirror_transfotmation>(image_path, 203);

                mirror_transformation_->read_file();

                mirror_transformation_->transformation();
                // 4. 旋转图像
                auto rotate_transformation_ = std::make_unique<rotate_transformation>(image_path, 30.0);

                rotate_transformation_->read_file();

                rotate_transformation_->transformation();

                std::cout<<"image transformation completed!"<<std::endl;

                // 给定阈值T
                auto t_threshold_ = std::make_unique<t_threshold_segmentation>(image_path, 100);

                t_threshold_->read_file();

                t_threshold_->output_segmentation_res();

                // 迭代阈值法
                auto iterative_threshold_ = std::make_unique<iterative_threshold_segmentation>(image_path);

                iterative_threshold_->read_file();

                iterative_threshold_->get_threshold();

                iterative_threshold_->output_segmentation_res();

                // ostu阈值法
                auto ostu_threshold_ = std::make_unique<ostu_threshold_segmentation>(image_path);

                ostu_threshold_->read_file();

                ostu_threshold_->get_threshold();

                ostu_threshold_->output_segmentation_res();

                std::cout<<"threshold segmentation completed!"<<std::endl;

                // 区域增长
                std::vector<int*> coors;

                coors.emplace_back(new int[2]{100, 100}); 

                coors.emplace_back(new int[2]{200, 20});

                auto region_growing_ = std::make_unique<region_growing>(image_path, coors);

                region_growing_->read_file();

                region_growing_->excute_growing();

                // 区域增强
                auto region_spliting_ = std::make_unique<region_spliting>(image_path);

                region_spliting_->read_file();

                region_spliting_->execute_spliting();

                std::cout<<"segmentation based on region completed!"<<std::endl;

                image_path = "../img/lena.bmp";

                // prewitt算子
                auto prewitt_detection_ = std::make_unique<prewitt_detection>(image_path);

                prewitt_detection_->read_file();

                prewitt_detection_->execute_detection();

                // sobel算子
                auto sobel_detection_ = std::make_unique<sobel_detection>(image_path);

                sobel_detection_->read_file();

                sobel_detection_->execute_detection();

                // log算子
                auto log_detection_ = std::make_unique<log_detection>(image_path);

                log_detection_->read_file();

                log_detection_->execute_detection();

                std::cout<<"edge detection completed!"<<std::endl;

                image_path = "../img/line.bmp";

                auto hough_transform_ = std::make_unique<hough_transformation>(image_path);

                hough_transform_->read_file();

                hough_transform_->convert_8_to_24();

                hough_transform_->execute_transformation();

                std::cout<<"hough transform completed!"<<std::endl;

                image_path = "../img/shape.bmp";

                auto zone_marker_ = std::make_unique<zone_marker>(image_path);

                zone_marker_->read_file();

                zone_marker_->convert_8_to_24();

                zone_marker_->execute_zone_mark();

                std::cout<<"zone marker completed!"<<std::endl;

                auto contour_extraction_ = std::make_unique<contour_extraction>(image_path);

                contour_extraction_->read_file();

                contour_extraction_->execute_contour_extract();

                std::cout<<"contour extraction completed!"<<std::endl;
            }else{
                std::map<int, std::string> num_to_path;
                std::string seper = ",";
                std::string seper2 = "-";
                auto res1 = split(input_string, seper);
                
                for(auto r : res1){
                    auto res2 = split(r,seper2);
                    num_to_path.emplace(std::pair<int,std::string>(atoi(res2.at(0).c_str()), res2.at(1)));
                }

                for(auto m : num_to_path){
                    std::string image_path = m.second;
                    switch(m.first){
                        case 1:{
                            auto image_process_ = std::make_unique<image_process>(image_path);

                            image_process_ ->read_file();
                            
                            image_process_->convert_to_gray();

                            image_process_->channel_split();

                            size_t pos = image_path.find_last_of(".");
                            if(pos == std::string::npos) {
                                throw std::exception();
                            }
                            std::string file_name = image_path.substr(0, pos);
                            std::string file_ext = image_path.substr(pos + 1);

                            file_name += "_gray";
                            std::string new_file_path = file_name + "." + file_ext;
                            image_process_->set_image_file(new_file_path);

                            image_process_->read_file();

                            image_process_->inverse_gray_color();

                            std::cout<<"file process completed!"<<std::endl;
                            break;
                        }
                        case 2:{
                            auto image_histogram_ = std::make_unique<image_histogram>(image_path);

                            image_histogram_->read_file();

                            image_histogram_->get_histogram_file();

                            image_histogram_->get_histogram_equaled();

                            std::cout<<"histogram processing completed!"<<std::endl;
                            break;
                        }
                        case 3:{
                            // 1. 均值滤波
                            auto spatial_domain_filtering_ = std::make_unique<average_filtering>(image_path, 3);

                            spatial_domain_filtering_->read_file();

                            spatial_domain_filtering_->do_filtering(spatial_domain_filtering_->get_border()); 

                            // // 2. 中值滤波
                            auto median_domain_filtering_ = std::make_unique<median_filtering>(image_path, 3);

                            median_domain_filtering_->read_file();

                            median_domain_filtering_->do_filtering(median_domain_filtering_->get_border());
                            std::cout<<"spatial domain filtering completed!"<<std::endl;
                            break;
                        }
                        case 4:{
                            // 1. 平移变换
                            auto translation_transformation_ = std::make_unique<translation_transfotmation>(image_path, 10, 10);

                            translation_transformation_->read_file();

                            translation_transformation_->transformation();
                            // 2. 缩放变换
                            auto scaling_transformation_ = std::make_unique<scaling_transformation>(image_path, 0.2, 0.2);

                            scaling_transformation_->read_file();

                            scaling_transformation_->transformation();
                            // 3. 镜像变换
                            auto mirror_transformation_ = std::make_unique<mirror_transfotmation>(image_path, 203);

                            mirror_transformation_->read_file();

                            mirror_transformation_->transformation();
                            // 4. 旋转图像
                            auto rotate_transformation_ = std::make_unique<rotate_transformation>(image_path, 30.0);

                            rotate_transformation_->read_file();

                            rotate_transformation_->transformation();

                            std::cout<<"image transformation completed!"<<std::endl;
                            break;
                        }
                        case 5:{
                            // 给定阈值T
                            auto t_threshold_ = std::make_unique<t_threshold_segmentation>(image_path, 100);

                            t_threshold_->read_file();

                            t_threshold_->output_segmentation_res();

                            // 迭代阈值法
                            auto iterative_threshold_ = std::make_unique<iterative_threshold_segmentation>(image_path);

                            iterative_threshold_->read_file();

                            iterative_threshold_->get_threshold();

                            iterative_threshold_->output_segmentation_res();

                            // ostu阈值法
                            auto ostu_threshold_ = std::make_unique<ostu_threshold_segmentation>(image_path);

                            ostu_threshold_->read_file();

                            ostu_threshold_->get_threshold();

                            ostu_threshold_->output_segmentation_res();

                            std::cout<<"threshold segmentation completed!"<<std::endl;
                            break;
                        }
                        case 6:{
                            // 区域增长
                            std::vector<int*> coors;

                            coors.emplace_back(new int[2]{100, 100}); 

                            coors.emplace_back(new int[2]{200, 20});

                            auto region_growing_ = std::make_unique<region_growing>(image_path, coors);

                            region_growing_->read_file();

                            region_growing_->excute_growing();

                            // 区域增强
                            auto region_spliting_ = std::make_unique<region_spliting>(image_path);

                            region_spliting_->read_file();

                            region_spliting_->execute_spliting();

                            std::cout<<"segmentation based on region completed!"<<std::endl;
                            break;
                        }
                        case 7:{
                            // prewitt算子
                            auto prewitt_detection_ = std::make_unique<prewitt_detection>(image_path);

                            prewitt_detection_->read_file();

                            prewitt_detection_->execute_detection();

                            // sobel算子
                            auto sobel_detection_ = std::make_unique<sobel_detection>(image_path);

                            sobel_detection_->read_file();

                            sobel_detection_->execute_detection();

                            // log算子
                            auto log_detection_ = std::make_unique<log_detection>(image_path);

                            log_detection_->read_file();

                            log_detection_->execute_detection();

                            std::cout<<"edge detection completed!"<<std::endl;
                            break;
                        }
                        case 8:{
                            auto hough_transform_ = std::make_unique<hough_transformation>(image_path);

                            hough_transform_->read_file();

                            hough_transform_->convert_8_to_24();

                            hough_transform_->execute_transformation();

                            std::cout<<"hough transform completed!"<<std::endl;
                            break;
                        }
                        case 9:{
                            
                            auto zone_marker_ = std::make_unique<zone_marker>(image_path);

                            zone_marker_->read_file();

                            zone_marker_->convert_8_to_24();

                            zone_marker_->execute_zone_mark();

                            std::cout<<"zone marker completed!"<<std::endl;
                            break;
                        }
                        case 10:{
                            auto contour_extraction_ = std::make_unique<contour_extraction>(image_path);

                            contour_extraction_->read_file();

                            contour_extraction_->execute_contour_extract();

                            std::cout<<"contour extraction completed!"<<std::endl;
                            break;
                        }
                    }
                }
                
            }
            return 0;
        }
    }

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
     * 5. 图像分割
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
     * 6. 区域分割
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
     * 7. 边缘检测
     */
    // prewitt算子
    // auto prewitt_detection_ = std::make_unique<prewitt_detection>(image_path);

    // prewitt_detection_->read_file();

    // prewitt_detection_->execute_detection();

    // sobel算子
    // auto sobel_detection_ = std::make_unique<sobel_detection>(image_path);

    // sobel_detection_->read_file();

    // sobel_detection_->execute_detection();

    // log算子
    // auto log_detection_ = std::make_unique<log_detection>(image_path);

    // log_detection_->read_file();

    // log_detection_->execute_detection();

    /**
     * 8. hough变换
     */
    // auto hough_transform_ = std::make_unique<hough_transformation>(image_path);

    // hough_transform_->read_file();

    // hough_transform_->convert_8_to_24();

    // hough_transform_->execute_transformation();

    /**
     * 9. 区域标记
     */
    // auto zone_marker_ = std::make_unique<zone_marker>(image_path);

    // zone_marker_->read_file();

    // zone_marker_->convert_8_to_24();

    // zone_marker_->execute_zone_mark();
    
    /**
     * 10. 轮廓提取
     */
    // auto contour_extraction_ = std::make_unique<contour_extraction>(image_path);

    // contour_extraction_->read_file();

    // contour_extraction_->execute_contour_extract();


    return 0;
}
