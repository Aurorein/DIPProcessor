#include "image_process.h"
#include <fstream>


void image_process::read_file(std::string file_path){

    std::ifstream file(file_path, std::ios::binary);

    char* file_header_buffer = new char[FILEHEADER_SIZE];
    file.read(file_header_buffer, FILEHEADER_SIZE);
    file.seekg(FILEHEADER_SIZE, std::ios::beg);

    bmp_file_.deserialize_file_header(file_header_buffer);
    delete file_header_buffer;

    char* info_header_buffer = new char[INFOHEDER_SIZE];
    file.read(info_header_buffer, INFOHEDER_SIZE);

    
}