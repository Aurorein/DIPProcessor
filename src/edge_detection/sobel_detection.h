#include "edge_detection.h"
#include "../common/edge_detection_operator/sobel_operator.h"

class sobel_detection : public edge_detection {
public:
    sobel_detection(std::string file_path) : edge_detection(file_path){
        oper_ = new sobel_operator();


    }

    std::string get_type() override{return "_sobel";}
};