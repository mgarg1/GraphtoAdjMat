#ifndef TEMPLATEMATCH_HPP
#define TEMPLATEMATCH_HPP

#include "../myUtilities/myUtilities.hpp"

#define THRES_LIMIT 100

// #define DIGITS 10
// dbImg *db[DIGITS];

struct dbImg{
    const char val;
    int offX,offY;
    int endX,endY;
    BMP intImg;

    dbImg(std::string,char);       
    dbImg() = delete;
    dbImg(const dbImg &);
    int compare(const BMP &img,CI x,CI y) const;
};


struct dataBase{
	std::vector<dbImg> v;
	dataBase() = delete;
	void insert(std::string fileLoc,char ch);
	char identifySingleCharAt(BMP &img,CI startX,CI startY,CI lenX,CI lenY) const;

	/*Yet to implement*/
	int searchCircle(const BMP &img,CI center_x,CI center_y,CI rad);
    int searchRectangle();
	
};

/*
void FloodFill2(BMP &img,const int i,const int j,dbImg &pattern,const RGBApixel cur_pixel);
void FloodFill2(BMP &img,const int i,const int j,dbImg &pattern,const RGBApixel curPixel,const RGBApixel destPixel);
void openFiles(std::string baseAddr);
void findNums(BMP &img);
*/
#endif