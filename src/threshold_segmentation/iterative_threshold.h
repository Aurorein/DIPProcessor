#include "image_segmentation.h"
#include "algorithm"

class iterative_threshold_segmentation : public image_segmentation {
private:


public:
    iterative_threshold_segmentation(std::string file_path) : image_segmentation(file_path){

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

        // 迭代求阈值
        uint8_t threshold1 = get_mid(data, 0, line_bytes * height).first;
        int mid_index = threshold1;
        uint8_t threshold2 = threshold1;
        do{
            threshold1 = threshold2;
            threshold2 = ( get_u(frequen_array, 0, mid_index) + get_u(frequen_array, mid_index + 1, 255) ) / 2;

            mid_index = threshold2;

        }while(threshold2 - threshold1 >= 10);
    
        threshold_ = threshold2;
        
    }

    // 中位数、数组下标
    std::pair<int, int> get_mid(uint8_t *arr, int index1, int index2){
        std::vector<std::pair<int, int>> values;

        for (int i = index1; i <= index2; i++) {
            values.push_back(std::make_pair(arr[i], i));
        }

        std::sort(values.begin(), values.end());

        int midIndex = values.size() / 2;
        int median = values[midIndex].first;
        int medianIndex = values[midIndex].second;

        return std::make_pair(median, medianIndex);
    }

    // 求得面积均值
    int get_u(uint32_t* arr, int index1, int index2){
        int up = 0;
        int down = 0;
        for(int i = index1; i <= index2; ++i){
            up += arr[i] * i;
            down += arr[i];
        }
        return up / down;

    }

    std::string get_type() override{return "_iterative_threshold";}

};