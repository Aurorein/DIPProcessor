#include "image_transformation.h"
#include "../common/transformational_matrix/mirror_matrix.h"

class mirror_transfotmation : public image_transformation{
public:
    mirror_transfotmation(std::string file_path, uint32_t width) : image_transformation(file_path){
        width_ = width;
        mirror_matrix* matrix = new mirror_matrix(width_);
        set_matrix(matrix);
    }    

    std::string get_type() override{return "mirror";}

private:
    uint32_t width_;
};