#include "image_segmentation.h"

class ostu_threshold_segmentation : public image_segmentation {
private:



public:
    ostu_threshold_segmentation(std::string file_path) : image_segmentation(file_path) {
    }

    void get_threshold() override{
        uint32_t *frequen_array = new uint32_t[256]{0};

        // 对灰度图每一个位图数据byte，将其统计到灰度频度数组中
        uint8_t* data = reinterpret_cast<uint8_t*>(bmp_file_.get_data());
        int32_t width = bmp_file_.get_width();
        int32_t height = bmp_file_.get_height();
        int32_t line_bytes = (width * 8 / 8 + 3) / 4 * 4;

        for(uint32_t i = 0; i < height; i++){
            for(uint32_t j = 0 ;j < width; j++){
                uint32_t gray_data = *(data + i*line_bytes + j);
                frequen_array[gray_data] ++;
            }
        }

        int res = INT32_MAX;
        int t = 1;

        // 求m1、m2、p1、p2
        for(int i = 1; i < 255; i ++) {
            int m1 = 0;
            int m2 = 0;
            int p1 = 0;
            int p2 = 0;
            for(int j = 0; j <= i; j ++) {
                m1 += frequen_array[j] * j; 
                p1 += frequen_array[j];
            }
            for(int j = i + 1 ; j <= 255; j ++) {
                m2 += frequen_array[j] * j;
                p2 += frequen_array[j];
            }

            if((p1 - p2) * (p1 - p2) < res){
                res =  (p1 - p2) * (p1 - p2);
                t = i;
            }
        }

        threshold_ = t;

    }

    std::string get_type() override{return "_ostu_threshold";}

}; 