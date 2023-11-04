#include "edge_detection_operator.h"


class prewitt_operator : public edge_detection_operator {
public:
    prewitt_operator() {
        operator_kernel_X_ = new int8_t[9]{0, 1, 1, -1 ,0 ,1 ,-1, -1, 0};
    
        operator_kernel_Y_ = new int8_t[9]{-1, -1, 0, -1, 0, 1, 0, 1, 1};

        sum_coeff_ = 9;
    }

};