#include "image_transformation.h"
#include "../common/transformational_matrix/translation_matrix.h"

class translation_transfotmation : public image_transformation{
public:
    translation_transfotmation(std::string file_path, uint32_t delta_x, uint32_t delta_y) : image_transformation(file_path){
        delta_x_ = delta_x;
        delta_y_ = delta_y;
        transformational_matrix* matrix = new translation_matrix(delta_x_, delta_y_);
        set_matrix(matrix);
    }    

    std::string get_type() override{return "translation";}

private:
    uint32_t delta_x_;
    uint32_t delta_y_;
};