#include "spatial_domain_filtering.h"
#include "../common/convolution_mask/median_filtering_convolution.h"

class median_filtering : public spatial_domain_filtering{
public:
    median_filtering(std::string file_path, int border) : spatial_domain_filtering(file_path){
        set_mask(new median_filtering_convolution(1 + border * 2 ,1 + border * 2));
        set_border(border);
    }

    ~median_filtering(){delete[] get_mask();}

    std::string get_type() override{return "median";}
};