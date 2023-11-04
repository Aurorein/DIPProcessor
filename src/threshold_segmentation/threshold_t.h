#include "image_segmentation.h"

class t_threshold_segmentation : public image_segmentation {
private:

public:
    t_threshold_segmentation(std::string file_path, uint8_t threshold) : image_segmentation(file_path){
        set_threshold(threshold);
    }

    void get_threshold() override{
        return;
    }

    std::string get_type() override{return "_t_threshold";}
};





