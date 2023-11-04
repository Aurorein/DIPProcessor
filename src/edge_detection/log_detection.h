#include "edge_detection.h"
#include "../common/edge_detection_operator/log_operator.h"

class log_detection : public edge_detection {
public:
    log_detection(std::string file_path) : edge_detection(file_path) {
        oper_ = new log_operator();

    }

    std::string get_type() override{return "_log";}

};