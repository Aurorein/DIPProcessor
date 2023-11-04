#include "transformational_matrix.h"

class scaling_matrix : public transformational_matrix{
public:
    scaling_matrix(float f_x, float f_y) : f_x_(f_x), f_y_(f_y){
        float *data = new float[3 * 3]{1.0 / f_x_, 0 , 0, 0, 1 / f_y_, 0, 0, 0, 1};
        set_data(data);
    }

private:
    float f_x_;
    float f_y_;
};