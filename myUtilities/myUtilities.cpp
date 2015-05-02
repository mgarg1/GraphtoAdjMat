// #include "./EasyBMP/EasyBMP.h"

#include "./myUtilities.hpp"

void SetPixel(RGBApixel *p1,RGBApixel *p2){
    SetPixel(p1,p2->Red,p2->Green,p2->Blue);
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

void RGBtoHSI(BMP &img){
    DFOR(i,theta,img.TellWidth(),img.TellHeight()){
        SetPixel(img(i,theta),(img(i,theta)->Green + img(i,theta)->Red + img(i,theta)->Blue)/3.0f);
    }
}