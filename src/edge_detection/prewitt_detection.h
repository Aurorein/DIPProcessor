#include "edge_detection.h"
#include "../common/edge_detection_operator/prewitt_operator.h"

class prewitt_detection : public edge_detection {
public:
    prewitt_detection(std::string file_path) : edge_detection(file_path) {
        oper_ = new prewitt_operator();
    }

    std::string get_type() override{return "_prewitt";}

};