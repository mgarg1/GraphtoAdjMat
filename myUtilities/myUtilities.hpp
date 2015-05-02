#ifndef MYUTILITIES_HPP
#define MYUTILITIES_HPP

#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>
#include <vector>
#include <array>
#include <climits>
#include <tuple>
#include <algorithm>
#include <functional>
#include <list>
#include <sstream>
#include <initializer_list>

#include "../EasyBMP/EasyBMP.h"

typedef const int CI;
#define PI 3.14159265
#define b2RAD (PI/180.0f)
enum{WHITE = 255,RED = 200,GREEN = 120,BLUE = 60,DARK = 30,BLACK = 0};


#define dbg(fmt, ...) \
    fprintf(stderr, "%s[%d]: " fmt "\n", __FILE__, __LINE__, ## __VA_ARGS__)

#define arrSize(var)  (int)(sizeof(var) / sizeof(*(var)))

#ifdef assert
#undef assert
#endif
                            
#define assert(expr,i) \
    if (!(expr)) { \
        std::cerr << "Error at line " << __LINE__ << ": " \
            <<  "assert(" #expr ");" <<"value: "<<i<< std::endl; \
        exit(EXIT_FAILURE); \
    }


#define DFOR(a,b,c,d) for(int a=0;a<c;++a)     \
                        for(int b=0;b<d;++b)

#define DFOR2(a,b,c,d,e,f) for(int a=0;a<c;a=a+e)     \
                        for(int b=0;b<d;b=b+f)

template<class T>
void SetPixelSizeSafe(BMP &img,int i,int j,T b1,T b2,T b3){
    if(img.TellWidth() > i && i >= 0
            && img.TellHeight() > j && j>=0)
        SetPixel(img(i,j),b1,b2,b3);
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

void SetPixel(RGBApixel *,RGBApixel *);

// template<class T>
// void SetPixel(RGBApixel *,T,T,T);

// void SetPixel(RGBApixel *,RGBApixel *);

// template<class T>
// void SetPixelSizeSafe(BMP &img,int,int,T,T,T);

// template <typename T>
// bool inRange(T,T,T);

bool operator==(const RGBApixel &,const RGBApixel &);

bool operator!=(const RGBApixel &,const RGBApixel &);

template <typename T>
bool inline inRange(T val,T range,T THRES){
    if(val < range + THRES && val > range - THRES)
        return true;
    return false;
}

/*
    returns the euclidean distance between the two points
*/
float dist(CI,CI,CI,CI);

void thresholding(BMP &img,const int thresValue);

void RGBtoHSI(BMP &);

#endif