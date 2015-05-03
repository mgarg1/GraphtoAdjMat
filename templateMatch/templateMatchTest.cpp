#include "../myUtilities/myUtilities.hpp"
#include "templateMatch.hpp"


char imgArr[] = {'0','1','2','3','4','5','6','7','8','9'/*,'A','B','C','D'*/};

// void nameVertices(BMP &img,std::list<circle> &circleVector){
//     BMP inImg(img);
//     maxEliminate(inImg);

//     for(auto &i:circleVector){
//         /*Yet to Correct*/
//         char ch = db1.identifySingleCharAt(inImg,i.center_x-(i.rad/2),i.center_y-(i.rad/2),i.rad,i.rad);
//         i.setName(ch);
//         dbg("char ::%c",ch);
//     }
// }


int main(int argc, char* argv[]){

    dataBase db1(".\\DB\\",imgArr,arrSize(imgArr));
  
    // dbImg *arr[arrSize(imgArr)];

    // const char *baseAddr = ".\\DB\\";

    // for(int i=0;i<10;++i){
    //     std::stringstream ss;
    //     ss<<baseAddr<<i<<".bmp";
    //     arr[i] = new dbImg(ss.str() , '0' + i);
    //     dbg("opened: %d",i);
    // }

    // if(argc < 3 || 
    //     atoi(argv[1]) < 0 || 
    //         atoi(argv[1]) >= arrSize(fun_arr) )
    // {
    //     std::cerr<<"incorrect args\n";
    //     std::cerr<<"usage: "<<argv[0]<<" <operation to perform 0-"<<arrSize(fun_arr)-1<<"> <input_file> [output_file]\n";
    //     std::cerr<<"please check README file for list of available operations\n";
    //     return -1;
    // }
    // openFiles(".\\DB\\");

    // BMP AnImage;
    // AnImage.ReadFromFile("input2.bmp");
    // thresholding(AnImage,THRES_LIMIT);
        
    // findNums(AnImage);
    // AnImage.WriteToFile("Output2.bmp");
    
    // BMP Output(AnImage);
    // fun_arr[atoi(argv[1])](Output);
    
    // if(argc == 4 && strlen(argv[3]) != 0)
    //     Output.WriteToFile(argv[3]);
    // else
    //     Output.WriteToFile("Output1.bmp");

    // printImg(Output);

    return 0;
}