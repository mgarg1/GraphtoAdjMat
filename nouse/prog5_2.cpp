#include "./EasyBMP/EasyBMP.h"
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include <initializer_list>
#define PI (3.1416)

enum{WHITE = 255,RED = 200,GREEN = 120,BLUE = 60,DARK = 30,BLACK = 0};

#define DFOR(a,b,c,d) for(int a=0;a<c;++a)     \
                        for(int b=0;b<d;++b)

#define DFOR2(a,b,c,d,e,f) for(int a=0;a<c;a=a+e)     \
                        for(int b=0;b<d;b=b+f)


#ifdef assert
#undef assert
#define assert(expr,i) \
    if (!(expr)) { \
        std::cerr << "Error at line " << __LINE__ << ": " \
            <<  "assert(" #expr ");" <<"value of i: "<<i<< std::endl; \
        exit(EXIT_FAILURE); \
    }

template<class T>
void SetPixel(RGBApixel *p1,T b1,T b2,T b3){
    
    auto chkLim = [](T &arg){
        if(arg < 0)
            arg = 0;
        if(arg > 255)
            arg = 255;
    };
    chkLim(b1);
    chkLim(b2);
    chkLim(b3);

    // assert(std::is_convertible<ebmpBYTE,T>::value);
    p1->Red = (ebmpBYTE)b1;
    p1->Green = (ebmpBYTE)b2;
    p1->Blue =(ebmpBYTE)b3;
}

template<class T>
void SetPixel(RGBApixel *p1,T b1){
    SetPixel(p1,b1,b1,b1);
}

void SetPixel(RGBApixel *p1,RGBApixel *p2){
    SetPixel(p1,p2->Green);
}

template<class T>
void SetPixelSafe(BMP &img,int i,int j,T b1,T b2=0,T b3=0){
    if(img.TellWidth() > i && i >= 0
            && img.TellHeight() > j && j>=0)
        SetPixel(img(i,j),b1,b2,b3);
}

void RGBtoHSI(BMP &img){
    DFOR(i,theta,img.TellWidth(),img.TellHeight()){
        SetPixel(img(i,theta),(img(i,theta)->Green + img(i,theta)->Red + img(i,theta)->Blue)/3.0f);
    }
}

//Bresenham Drawline algorithm
void Drawline(BMP &img,int x1,int y1,int x2,int y2,int color){

    auto chk_x = [](int &x_coord,const int lim){
        if(x_coord < 0)
            x_coord = 0;
        if(x_coord >= lim)
            x_coord = lim - 1;
    };

    chk_x(x1,img.TellWidth());
    chk_x(y1,img.TellHeight());
    chk_x(x2,img.TellWidth());
    chk_x(y2,img.TellHeight());

    int delta_x = x2 - x1;
    int delta_y = y2 - y1;
    
    char const ix = (delta_x > 0) - (delta_x < 0);
    char const iy = (delta_y > 0) - (delta_y < 0);
    
    delta_x = std::abs(delta_x) << 1;
    delta_y = std::abs(delta_y) << 1;
 
    SetPixel(img(x1,y1),color,0,0);
    // plot(x1, y1);
 
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
 
        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing
            error += delta_y;
            x1 += ix;
        
            SetPixel(img(x1,y1),color,0,0);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
 
        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing
 
            error += delta_x;
            y1 += iy;
            SetPixel(img(x1,y1),color,0,0);
        }
    }
}

void DrawCross(BMP &img,int x1, int y1,const int color){
    const int len = 2;
    Drawline(img,x1-len,y1-len,x1+len,y1+len,color);
    Drawline(img,x1-len,y1+len,x1+len,y1-len,color);
}

const int N = 8;

struct block{
    int xCoord;
    int yCoord;
    RGBApixel *values[N*N];
};

void makeNxNsubimages(BMP &img,std::vector<block> &blockVector){

    const int W = img.TellWidth();
    const int H = img.TellHeight();
    const int dim = N;

    DFOR2(a,b,W-(dim-1),H-(dim-1),dim,dim){

        block b1;
        int i=0;

        DFOR(x,y,dim,dim){
            const int rel_x = a+x;
            const int rel_y = b+y;
            b1.values[i++] = img(rel_x,rel_y); 
        }

        //tried to point to the center pixel location but is not
        //possible in case of even dimensions so pointed to first 
        //pixel of the inner square
        b1.xCoord = a + dim/2;
        b1.yCoord = b + dim/2;
        blockVector.push_back(b1);
    }    
}


void copyBlocks(block &b1,const block &b2){
    const int SZ = N*N;

    for(int i=0;i<SZ;++i){
        SetPixel(b1.values[i],b2.values[i]->Green);
    }
}

/*
    This Function uses Mean Absolute Difference to COmpare two blocks
*/
float compareBlocks(const block &b1,const block &b2){
    const int SZ = N*N;
    float sum = 0;

    for(int i=0;i<SZ;++i){
        sum += abs(b1.values[i]->Green - b2.values[i]->Green);
    }

    return sum/(float)SZ;
}

/*
    This Function return the index of the nearest block to the source block
    within a fixed window size
*/

size_t searchNearestBlock(const BMP &img,const std::vector<block> &img1Blocks,
                    const std::vector<block> &img2Blocks, const size_t index){

    //window size for the blocks to be compared
    const int WindowSize = 9;

    const size_t startIndex = (index - WindowSize/2) > 0 ?
                            (index - WindowSize/2) : 0; 

    const size_t endIndex = (index + WindowSize/2) >= img1Blocks.size() ?
                        img1Blocks.size() - 1 : (index + WindowSize/2);

    size_t minIndex = index;
    float minValue = 99999;

    for (size_t i = startIndex ; i < endIndex ; ++i){    
        float currValue = compareBlocks(img1Blocks.at(index),img2Blocks.at(i));
        // std::cout<<currValue<<std::endl;    
        if(currValue < minValue){
            minValue = currValue;
            minIndex = i;
        }
    }
    return minIndex;
}

/* 
    This function finds the motion vector and the mark them 
    by using lines and cross

    here img1 represents - old frame and 
    img2 repressents the new frame
*/
void motionVector(BMP &img1,BMP &img2,const int THRES){
    RGBtoHSI(img1);
    RGBtoHSI(img2);
    std::vector<block> img1Blocks;
    std::vector<block> img2Blocks;

    makeNxNsubimages(img1,img1Blocks);
    makeNxNsubimages(img2,img2Blocks);

    
    int count=0;
    std::cout<<"total block count: "<<img1Blocks.size()<<std::endl;
    for (size_t i = 0; i < img1Blocks.size(); ++i)
    {
        // SetPixelSafe(img1,img1Blocks.at(i).xCoord,img1Blocks.at(i).yCoord,255);
        if(compareBlocks(img1Blocks.at(i) , img2Blocks.at(i)) > THRES){
            
            size_t minIndex = searchNearestBlock(img1,img1Blocks,img2Blocks,i);
            if(minIndex != i){
                // std::cout<<minIndex<<std::endl;
                /*cross indicating the begining of the motion*/
                count++;
                DrawCross(img1,img1Blocks.at(i).xCoord,img1Blocks.at(i).yCoord,RED);
                Drawline(img1,
                            img1Blocks.at(i).xCoord,img1Blocks.at(i).yCoord,
                                img2Blocks.at(minIndex).xCoord,img2Blocks.at(minIndex).yCoord,RED);
            }
        }
    }
    std::cout<<"block moved: "<<count<<std::endl;
}

void difference(BMP &img1,BMP &img2,const int THRES = 0){
    RGBtoHSI(img1);
    RGBtoHSI(img2);
    
    DFOR(i,j,img1.TellWidth(),img1.TellHeight()){
        SetPixel(img1(i,j),img1(i,j)->Green - img2(i,j)->Green);
    }
}

void compDifference(BMP &img1,BMP &img2,const int THRES = 0){
    RGBtoHSI(img1);
    RGBtoHSI(img2);
    std::vector<block> img1Blocks;
    std::vector<block> img2Blocks;

    makeNxNsubimages(img1,img1Blocks);
    makeNxNsubimages(img2,img2Blocks);

    
    for (size_t i = 0; i < img1Blocks.size(); ++i)
    {
        if(compareBlocks(img1Blocks.at(i) , img2Blocks.at(i)) > THRES){
            
            size_t minIndex = searchNearestBlock(img1,img1Blocks,img2Blocks,i);
            if(minIndex != i){
                copyBlocks(img1Blocks.at(i),img2Blocks.at(minIndex));
            }
        }
    }
    
    DFOR(i,j,img1.TellWidth(),img1.TellHeight()){
        SetPixel(img1(i,j),img1(i,j)->Green - img2(i,j)->Green);
    }  
}

void (*fun_arr[]) (BMP &,BMP &,const int) = {
/*0*/   motionVector
/*1*/   ,difference
/*2*/   ,compDifference
};

#define arrSize(var)  (int)(sizeof(var) / sizeof(*(var)))

int main(int argc, char* argv[]){

    if(argc < 5 || 
        atoi(argv[1]) < 0 || 
            atoi(argv[1]) >= arrSize(fun_arr) )
    {
        std::cerr<<"incorrect args\n";
        std::cerr<<"usage: "<<argv[0]<<" <operation to perform 0-"<<arrSize(fun_arr)-1<<"> <input_file1> <input_file2> <THRESHOLD> [output_file]\n";
        std::cerr<<"please check README file for list of available operations\n";
        return -1;
    }

    // qBIa();

    BMP FrameImg1;
    FrameImg1.ReadFromFile(argv[2]);
    
    BMP FrameImg2;
    FrameImg2.ReadFromFile(argv[3]);
    
    BMP Output(FrameImg1);
    fun_arr[atoi(argv[1])](Output,FrameImg2,atoi(argv[4]));
    
    if(argc == 6 && strlen(argv[5]) != 0)
        Output.WriteToFile(argv[5]);
    else
        Output.WriteToFile("Output1.bmp");

    // printImg(Output);

    return 0;
}
#endif