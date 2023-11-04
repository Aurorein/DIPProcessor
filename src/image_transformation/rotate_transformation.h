#include "image_transformation.h"
#include "../common/transformational_matrix/rotate_matrix.h"

class rotate_transformation : public image_transformation{
public:
    rotate_transformation(std::string file_path, float theta) : image_transformation(file_path) {
        theta_ = theta;
        transformational_matrix* matrix = new rotate_matrix(theta_);
        set_matrix(matrix);
    }

    std::string get_type() override{return "rotate";}

private:
    float theta_;
};