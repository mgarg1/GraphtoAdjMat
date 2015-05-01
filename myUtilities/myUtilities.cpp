// #include "./EasyBMP/EasyBMP.h"

#include "./myUtilities.hpp"

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
void SetPixelSizeSafe(BMP &img,int i,int j,T b1,T b2,T b3){
    if(img.TellWidth() > i && i >= 0
            && img.TellHeight() > j && j>=0)
        SetPixel(img(i,j),b1,b2,b3);
}

bool operator==(const RGBApixel &p1,const RGBApixel &p2){
    return (p1.Red == p2.Red && p1.Green == p2.Green && p1.Blue == p2.Blue);
}

bool operator!=(const RGBApixel &p1,const RGBApixel &p2){
    return !(p1==p2);
}

/*
    returns the euclidean distance between the two points
*/
float dist(const int x1,const int y1,const int x2,const int y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

void thresholding(BMP &img,const int thresValue){
    assert(thresValue > 0 && thresValue < 255 , thresValue);
    
    DFOR(i,theta,img.TellWidth(),img.TellHeight()){
        if(img(i,theta)->Green > thresValue)
            SetPixel(img(i,theta),255);
        else
            SetPixel(img(i,theta),0);
    }
}
