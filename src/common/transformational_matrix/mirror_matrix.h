#include "transformational_matrix.h"

class mirror_matrix : public transformational_matrix{
public:
    mirror_matrix(uint32_t width) : width_(width){
        float *data = new float[3 * 3]{-1, 0 ,width_ - 1, 0, 1, 0, 0, 0, 1};
        set_data(data);
    }

private:
    uint32_t width_;
};