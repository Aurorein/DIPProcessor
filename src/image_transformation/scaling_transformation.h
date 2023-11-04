#include "image_transformation.h"
#include "../common/transformational_matrix/scaling_matrix.h"

class scaling_transformation : public image_transformation{
public:
    scaling_transformation(std::string file_path, float f_x, float f_y) : image_transformation(file_path) {
        f_x_ = f_x;
        f_y_ = f_y;
        transformational_matrix* matrix = new scaling_matrix(f_x_, f_y_);
        set_matrix(matrix);
    }

    std::string get_type() override{return "scaling";}

private:
    float f_x_;
    float f_y_;
};