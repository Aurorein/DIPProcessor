#include "edge_detection_operator.h"


class sobel_operator : public edge_detection_operator {
public:
    sobel_operator() {
        operator_kernel_X_ = new int8_t[9]{-1, -2, -1, 0 ,0 ,0 ,1, 2, 1};
        
        operator_kernel_Y_ = new int8_t[9]{-1, 0, 1, -2, 0, 2, -1, 0, 1};

        sum_coeff_ = 9;
    }



};