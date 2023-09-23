#include "spatial_domain_filtering.h"
#include "../common/convolution_mask/average_filtering_convolution.h"

class average_filtering : public spatial_domain_filtering{
public:
    average_filtering(std::string file_path, int border) : spatial_domain_filtering(file_path){
        set_mask(new average_filtering_convolution(1 + border * 2 ,1 + border * 2));
        set_border(border);
    }

    ~average_filtering(){delete[] get_mask();}

    std::string get_type() override{return "average";}

private:

};