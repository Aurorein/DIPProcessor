#include "transformational_matrix.h"

class translation_matrix : public transformational_matrix{
public:
    translation_matrix(uint32_t delta_x, uint32_t delta_y) : delta_x_(delta_x), delta_y_(delta_y){
        float *data = new float[3 * 3]{1, 0 ,delta_x_, 0, 1, delta_y_, 0, 0, 1};
        set_data(data);
    }

private:
    uint32_t delta_x_;
    uint32_t delta_y_;
};