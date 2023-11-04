#include "transformational_matrix.h"
#include <cmath>

class rotate_matrix : public transformational_matrix{
public:
    rotate_matrix(float theta) : theta_(theta * M_PI / 180.0){
        float *data = new float[3 * 3]{cos(theta_), -sin(theta_) ,0, sin(theta_), cos(theta_), 0, 0, 0, 1};
        set_data(data);
    }

private:
    float theta_;
};