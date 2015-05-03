#include "../myUtilities/myUtilities.hpp"

int main(int argc, char* argv[]){

    if(argc!=3){
        std::cerr<<"incorrect args\n";
        std::cerr<<"usage: "<<argv[0]<<"<input_file> <output_file>\n";
        return -1;
    }

    BMP img;
    img.ReadFromFile(argv[1]);
    thresholding(img,20);
    img.WriteToFile(argv[2]);
    
    return 0;
}