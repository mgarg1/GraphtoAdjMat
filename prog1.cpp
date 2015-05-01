#include "./myUtilities/myUtilities.hpp"

// void RGBtoHSI(BMP &img){
//     DFOR(i,theta,img.TellWidth(),img.TellHeight()){
//         SetPixel<float>(img(i,theta),(img(i,theta)->Green + img(i,theta)->Red + img(i,theta)->Blue)/3.0f);
//     }
// }

// void sobelOperator(BMP &img){

//     const BMP inImg(img);
//     // RGBtoHSI(cImg);
//     // const BMP &inImg = cImg;
//     // mask mX(3,3,0);
//     int mX[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
//     // mask mY(3,3,0);
//     int mY[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

//     //image traversal   
//     for(int i=0;i<inImg.TellWidth();i++){
//         for(int theta=0;theta<inImg.TellHeight();theta++){

//             int sumX=0;
//             int sumY=0;

//             //mask traversal
//             int dim = 3;
//             for(int k = 0; k < dim ; ++k){
//                 for(int l = 0; l < dim; ++l){
                    
//                     const int rel_i = i - dim/2 + k ;
//                     const int rel_j = theta - dim/2 + l ;
                    
//                     if(!(rel_i < 0 || rel_j < 0 || rel_i > inImg.TellWidth()-1 || rel_j > inImg.TellHeight()-1)){
//                         sumX += (int)((inImg.GetPixel(rel_i,rel_j).Green) * (mX[k][l]));
//                         sumY += (int)((inImg.GetPixel(rel_i,rel_j).Green) * (mY[k][l]));
//                     }
//                 }
//             }
//             SetPixel(img(i,theta),(sqrt(sumX*sumX + sumY*sumY)));
//         }
//     }
// }

//Bresenham Drawline algorithm
void Drawline(BMP &img,int x1,int y1,int x2,int y2,int color){

    auto chk_x = [](int &coord,const int lim){
        coord = coord >= 0 ? coord :0; 
        coord = coord <= lim-1 ? coord :lim-1;
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

template<class T>
void SetPixel2(const RGBApixel refpi,RGBApixel *p1,T b1,T b2,T b3){
    if(refpi.Green > 250)
        SetPixel(p1,b1,b2,b3);
}

void DrawlineSegment(const BMP &origImg,BMP &img,int x1,int y1,int x2,int y2,int color){
     
    auto chk_x = [](int &coord,const int lim){
        coord = coord >= 0 ? coord :0; 
        coord = coord <= lim-1 ? coord :lim-1;
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
 
    SetPixel2(origImg.GetPixel(x1,y1),img(x1,y1),0,0,color);
    // plot(x1, y1);
 
    if (delta_x >= delta_y){
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
 
        while (x1 != x2){
            if ((error >= 0) && (error || (ix > 0))){
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing
            error += delta_y;
            x1 += ix;
            SetPixel2(origImg.GetPixel(x1,y1),img(x1,y1),color,0,0);
        }
    }
    else{
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
 
        while (y1 != y2){
            if ((error >= 0) && (error || (iy > 0))){
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing
            error += delta_x;
            y1 += iy;
            SetPixel2(origImg.GetPixel(x1,y1),img(x1,y1),color,0,0);
        }
    } 
}

std::tuple<int,int,int,int> 
polarToCoord(const BMP &img,const int rad,const double theta){
    
    const int img_w = img.TellWidth();
    const int img_h = img.TellHeight();
    const int center_x = img_w/2;
    const int center_y = img_h/2;
    //converting to radians
    const double thetaRad = theta * b2RAD;

    int x1,y1,x2,y2;
    std::tuple<int,int,int,int> t1;  
    if(theta >= 45 && theta <= 135){ 
        //start to end
        x1 = 0; 
        x2 = img_w; 

        //y = (rad - x cos(theta)) / sin(theta)  
        y1 = round((rad - ((x1 - center_x ) * cos(thetaRad))) / sin(thetaRad));  
        y2 = round((rad - ((x2 - center_x ) * cos(thetaRad))) / sin(thetaRad));  
        
        //apply offsets
        y1 += center_y; 
        y2 += center_y;
    }  
    else{
        //start to end
        y1 = 0; 
        y2 = img_h;

        //x = (rad - y sin(theta)) / cos(theta);  
        x1 = round((rad - ((y1 - center_y ) * sin(thetaRad))) / cos(thetaRad));  
        x2 = round((rad - ((y2 - center_y ) * sin(thetaRad))) / cos(thetaRad));  
        
        //apply offsets
        x1 += center_x; 
        x2 += center_x;
    }
    // std::cout<<theta<<"\t"<<rad<<"\t";
    // std::cout<<x1<<","<<y1<<" to "<<x2<<","<<y2<<std::endl;
    return std::make_tuple(x1,y1,x2,y2);
    // DrawlineSegment(img,x1,y1,x2,y2,RED);
}

#define test_p(x,y) ( (x) >= img.TellWidth()-1  ) || ( (x) < 0 ) || \
                    ( (y) >= img.TellHeight()-1 ) || ( (y) < 0 ) || \
                     (img.GetPixel((x),(y)) != cur_pixel) 
                    

std::pair<int,int> minX,maxX,minY,maxY;


/*this FloodFill algorithm return true if it is a valid distinct region*/
void FloodFill2(BMP &img,const int i,const int j,
                std::vector<RGBApixel *> &v,const RGBApixel &cur_pixel){

    if(test_p(i,j)){
        return;
    }
    SetPixel(img(i,j),0,255,0);
    v.push_back(img(i,j));    

    // grp_no[i][j] = group;
    // const ebmpBYTE temp = img.GetPixel(i,j).Green
    //return 
    if(i < minX.first)
        minX = {i,j};
    if(i > maxX.first)
        maxX = {i,j};
    if(j < minY.second)
        minY = {i,j};
    if(j > maxY.second)
        maxY = {i,j};


    FloodFill2(img,i+1,j,v,cur_pixel);
    FloodFill2(img,i,j+1,v,cur_pixel);
    FloodFill2(img,i,j-1,v,cur_pixel);
    FloodFill2(img,i-1,j,v,cur_pixel);
    
    FloodFill2(img,i+1,j+1,v,cur_pixel);
    FloodFill2(img,i+1,j-1,v,cur_pixel);
    FloodFill2(img,i-1,j+1,v,cur_pixel);
    FloodFill2(img,i-1,j-1,v,cur_pixel);
    
    // FloodFill2(img,i,j-1,v,cur_pixel);
    // FloodFill2(img,i-1,j,v,cur_pixel);
}

struct lineSeg{
    int x1,y1,x2,y2;
    void print(){
        printf("Line Segment : %d,%d to %d,%d\n",x1,y1,x2,y2);
    }
};

// typedef const int CI

// void FloodFill3(BMP &img,CI i,CI j,int **grp_no,CI group){

//     const int h = img.TellHeight();
//     const int w = img.TellWidth();
//     const RGBApixel curPixel = *img(i,j);

//     std::function<void (int,int)> intRec = [&](int x,int y){
      
//         if(x >= w-1 || x < 0 || y >= h-1 || y < 0 ||
//             img.GetPixel(x,y) != curPixel || grp_no[x][y] != 0)
//             return;

//         // SetPixel(img(x,y),255,0,0);
//         // pattern.offset.push_back({pattern.startX-x,pattern.startY-y});
//         grp_no[i][j] = group;

//         intRec(x+1,y);
//         intRec(x,y+1);
//         intRec(x,y-1);
//         intRec(x-1,y);

//         intRec(x+1,y+1);
//         intRec(x+1,y-1);
//         intRec(x-1,y+1);
//         intRec(x-1,y-1);
//     };

//     intRec(i,j);
// }

// void connectedComponent3(BMP &img){

//     int groups = 1;
//     const int w = img.TellWidth();
//     const int h = img.TellHeight();

//     //allocating 2-d arrays
//     int **grp_no;
//     grp_no = new int*[w]; 
//     for (int i = 0; i < w; ++i){
//         grp_no[i] = new int[h]; 
//         memset(grp_no[i],0,h*sizeof(int));
//     }
    
//     //asuming img(0,0)'s pixel is background
//     RGBApixel temp = *img(0,0);
//     // SetPixel(&temp,0);

//     DFOR(i,j,w,h){
//         if(*img(i,j) != temp && (grp_no[i][j] == 0)){
//             FloodFill3(img,i,j,grp_no,groups);
//             groups++;
//         }
//     }

//     std::cout<<groups<<std::endl;
//     int grp_cnt[groups];
    
//     DFOR(i,j,w,h){
//         grp_cnt[grp_no[i][j]]++;
//     }

//     //usage specific code
//     {
//         int min=0,max=0;
//         for(int i=1;i<groups;i++){
//             if(grp_cnt[i] > grp_cnt[max]){
//                 max = i;
//             }
//             // if(grp_cnt[i] < grp_cnt[min]){
//             //     min = i;
//             // }
//         }
//     }

//     for(int i=0;i<width;i++){
//         for(int j=0;j<height;j++){
//             SetPixel(img(i,j),grp_cnt[grp_no[i][j]]);
//         }
//     }

//     for (int i = 0; i < width; ++i){
//         delete[] grp_no[i];
//     }
//     delete[] grp_no;
 
// }



void connectedComponent(BMP &img,std::vector<lineSeg> &lsv){
    
    const int THRES = 10;
    int groups = 1;
    const int width = img.TellWidth();
    const int height = img.TellHeight();
    std::vector<RGBApixel *> v;
    // std::cout<<"in here\n";
    DFOR(i,j,width,height){
        if(img(i,j)->Red != 0){
            // std::cout<<"found one grp\n";
            const RGBApixel temp = img.GetPixel(i,j);
            minX = {100000,11};
            maxX = {-100000,11};
            minY = {11,100000};
            maxY = {11,-100000};

            FloodFill2(img,i,j,v,temp);
            // std::cout<<v.size()<<" "<<i<<" "<<j<<"\n";
            
            if(v.size() < THRES){
                //paint black
                for(auto temp_pixel:v){
                    SetPixel(temp_pixel,0);
                }
            }else if(minX.first != maxX.first){
                //both minX and maxX will be the end-points
                lsv.push_back(lineSeg{minX.first,minX.second,maxX.first,maxX.second});
            }
            else{
                //think of other end-points
                lsv.push_back(lineSeg{minY.first,minY.second,maxY.first,maxY.second});
            }

            v.clear();
        }
    }
}

struct line{
    int rad;
    int theta;
    
    int x1,y1,x2,y2;

    void print(){
        std::cout<<rad<<"\t"<<theta<<"\n";
    }
};

struct circle{
    int rad;
    int center_x;
    int center_y;
    
    void print(){
        std::cout<<rad<<"\t"<<center_x<<"\t"<<center_y<<"\n";
    }
};

void HughTransformLine(const BMP &origImg,BMP &img,std::list<line> &linesVector){ 

    const int THRES2 = 250;
    const int THRES3 = 80;
    // const int THRES3 = 400;

    // RGBtoHSI(img);
    // sobelOperator(img);
    // thresholding(img,THRES1);

    const int h = origImg.TellHeight();
    const int w = origImg.TellWidth();
    const double center_x = w/2;  
    const double center_y = h/2;  
    
    //calculate the maximum possible i from the center_x,center_y 
    const int max_r = (int)round((sqrt(2.0) * (double)(h>w?h:w)) / 2.0);  
    
    //assuming 180 degrees on both sides So r can take both +ve and -ve values
    const int _accu_x = max_r*2;
    const int _accu_y = 180;
    
    //Create the accumulator  
    int accumulator[_accu_x][_accu_y];

    DFOR(x,y,_accu_x,_accu_y){  
        accumulator[x][y] = 0;        
    }

    //incrmenting accumulator pixels
    DFOR(x,y,w,h){
        if(origImg.GetPixel(x,y).Green > THRES2){  
            for(int theta=0;theta<180;theta++){  
              //i = xcos(theta) + ysin(theta);
              int rad = round(((x - center_x) * cos(theta * b2RAD)) + ((y - center_y) * sin(theta * b2RAD)));  
              // i + max_r to make everything positive
              accumulator[max_r + rad][theta]++;  
            }  
        }      
    }

    const int DIM = 9; 
    // std::vector<int> radii,thetas;
    // std::tuple<int,int,int,int> t;

    //Search if the point is a local Maxima in accumulator
    DFOR(rad,theta,_accu_x,_accu_y){
        if(accumulator[rad][theta] > THRES3){
                // std::cout<<"found max\n";
            
            int max = accumulator[rad][theta];
            DFOR(k,l,DIM,DIM){
                const int rel_i = rad - DIM/2 + k ;
                const int rel_j = theta - DIM/2 + l ; 
            
                if(!(rel_i < 0 || rel_j < 0 || rel_i > _accu_y-1 || rel_j > _accu_x-1)){
                    if(max < accumulator[rel_i][rel_j]){
                        // max = accumulator[rel_i][rel_j];
                        goto label1;
                    }                                    
                }
            }
            //if current accumulator is a local maxima
            //make line for these points    
            // radii.push_back((rad - max_r));
            // thetas.push_back(theta);
            linesVector.push_back(line{rad-max_r,theta});
            // t = polarToCoord(origImg,rad - max_r,theta);
            // DrawlineSegment(origImg,img,std::get<0>(t),std::get<1>(t),
            //                 std::get<2>(t),std::get<3>(t),RED);
            // DrawlineFromPolar(img,rad - max_r,theta,RED);            
            label1:;
        }
    }
    // identifyLines(img,radii,thetas);
}

// void DrawCircle(BMP &img,const int x0, const int y0, const int radius,
//                     const int rVal,const int gVal=0,const int bVal=0)
// {
//   int x = radius;
//   int y = 0;
//   int radiusError = 1-x;
//   // int rVal=255,gVal=0,bVal=0;

//   while(x >= y)
//   {
    
//     SetPixelSizeSafe(img,x+x0,y+y0,rVal,gVal,bVal);
//     SetPixelSizeSafe(img,x+x0,-y+y0,rVal,gVal,bVal);
//     SetPixelSizeSafe(img,-x+x0,y+y0,rVal,gVal,bVal);
//     SetPixelSizeSafe(img,-x+x0,-y+y0,rVal,gVal,bVal);
    
//     SetPixelSizeSafe(img,y+x0,x+y0,rVal,gVal,bVal);
//     SetPixelSizeSafe(img,-y+x0,x+y0,rVal,gVal,bVal);
//     SetPixelSizeSafe(img,y+x0,-x+y0,rVal,gVal,bVal);
//     SetPixelSizeSafe(img,-y+x0,-x+y0,rVal,gVal,bVal);
//     y++;

//     if (radiusError<0)
//     {
//       radiusError += 2 * y + 1;
//     }
//     else
//     {
//       x--;
//       radiusError += 2 * (y - x) + 1;
//     }
//   }
// }

void HughTransformCircleWithRad(const BMP &origImg,BMP &img,const int rad /*= 50*/,
    std::list<circle> &circleVector,const int THRES2 = 250,const int THRES3 = 100){ 
    
    // const BMP &img = Oimg;
    const int h = origImg.TellHeight();
    const int w = origImg.TellWidth();

    const int _accu_x = w;
    const int _accu_y = h;
 
/*
    accumulator stores the probab of center
    at x,y coordinate
*/
    int accumulator[_accu_x][_accu_y];
        
    DFOR(x,y,_accu_x,_accu_y){
        accumulator[x][y] = 0;
    }

    //incrmenting accumulator pixels
    DFOR(x,y,w-1,h-1){       
        if(origImg.GetPixel(x,y).Green > THRES2){ 
            /*x-coord of the circle can be in +-rad distance only
              Limits on the range of x and y coordinates of the center
            */
            int xstart = ((x - rad) > 0 )? x - rad : 0 ;
            int xend   = ((x + rad) < w )? x + rad : w ;
            int ystart = ((y - rad) > 0 )? y - rad : 0 ;
            int yend   = ((y + rad) < h )? y + rad : h ;

            for(int a=xstart;a<xend;++a){ //changing x-coord of the center of circle
                //(x-a)2 + (y-b)2 = r2
                const int temp = rad*rad - (x-a)*(x-a);
                if (temp > 0){
                    int b = (float)y - sqrt(temp);
                    if (b >= ystart && b < yend){
                        accumulator[a][b]++;
                    }
                }
            }  
        }
    }

    
   const int DIM = 9; 
    //Search if the point is a local Maxima in accumulator
    DFOR(x,y,_accu_x,_accu_y){
        if(accumulator[x][y] > THRES3){
            // std::cout<<"inside here";
            
            int max = accumulator[x][y];
            DFOR(k,l,DIM,DIM){
                const int rel_i = x - DIM/2 + k ;
                const int rel_j = y - DIM/2 + l ; 
            
                if(!(rel_i < 0 || rel_j < 0 || rel_i > _accu_x-1 || rel_j > _accu_y-1)){
                    if(max < accumulator[rel_i][rel_j]){
                        max = accumulator[rel_i][rel_j];
                        goto label2;
                    }                                    
                }
            }
            // DrawCircle(img,x,y,rad,RED);
            circleVector.push_back(circle{rad,x,y});
            //if current accumulator is a local maxima
            // if(max == accumulator[x][y]){
            // }
            label2:;
        }
    }
    // DrawCircle(Oimg,100,100,rad,RED);
}

void HughTransformCircle(const BMP& origImg,BMP &img,
    std::list<circle> &circleVector){ 
    const int THRES1 = 200;
    const int THRES2 = 250;
    const int THRES3 = 50;

    //For smaller Circles
    for(int i=50;i<=70;i++){
        HughTransformCircleWithRad(origImg,img,i,circleVector,THRES2,THRES3);    
    }
}

void filterCircles(std::list<circle> &circleVector){
    auto i = circleVector.begin();
    while(i != circleVector.end()){
        auto j = i;
        ++j;
        while(j != circleVector.end()){
            if(dist(i->center_x, i->center_y, j->center_x, j->center_y) 
                                        < (i->rad + j->rad)){
                j = circleVector.erase(j);
            }
            else
                ++j;                  
        }
        ++i;
    }
}

template <typename T>
bool inline inRange(T val,T range,T THRES){
    if(val < range + THRES && val > range - THRES)
        return true;
    return false;
}


void filterLines(std::list<line> &linesVector){
    auto i = linesVector.begin();
    const int THRESH_FILTER_LINE = 5;

    while(i != linesVector.end()){
        auto j = i;
        ++j;
        while(j != linesVector.end()){
            if(inRange(i->rad,j->rad,THRESH_FILTER_LINE)
                        && inRange( i->theta, j->theta, THRESH_FILTER_LINE)){
                j = linesVector.erase(j);
            }
            else
                ++j;                  
        }
        ++i;
    }   
}


#if 0
bool edgeExist(const BMP &img,const circle &c1,const circle &c2){
    
    const int center_x = img.TellWidth()/2;  
    const int center_y = img.TellHeight()/2;  
/*
    auto shiftOrigin = [&](int &x,int &y){
        x = center_x - x;
        y = center_y - y;
    };

    circle tc1 = c1;
    circle tc2 = c2;

    shiftOrigin(tc1.center_x,tc1.center_y);
    shiftOrigin(tc2.center_x,tc2.center_y);

    double m1 = (tc1.center_y - tc2.center_y)*1.0/
                  (tc1.center_x - tc2.center_x);
    
    double angle = atan (m1) * 180 / PI;
    double pDist = (tc1.center_y - m1 * tc1.center_x) * 1.0 / sqrt(m1*m1 + 1);
    */
    //slope of line joining the centers of two circles
    double m1 = (c1.center_y - c2.center_y)*1.0/
                (c1.center_x - c2.center_x);
    
    //slope of line joining the center of a circle and the center of the image
    double m2 = (c1.center_y - center_y)*1.0/
                (c1.center_x - center_x);
    
    //convert slope to angle in degrees
    double angle = atan (m1) * 180 / PI;
    
    //distance between center of one circle and the center of the image
    float pDist = dist(c1.center_x,c1.center_y,center_x,center_y);
    

    pDist = pDist * sin(atan(m1)-atan(m2));

    // std::cout<<round(pDist)<<"\t"<<90-abs(round(angle))<<"\n";
    std::cout<<round(pDist)<<"\t"<<90-round(angle)<<"\n";
    return true;
}
#endif

/* this function check whether there exists a line between two given circles*/
bool edgeExist(const BMP &img,const circle &c1,const circle &c2,const std::list<line> &linesVector){
    
    int center_x = img.TellWidth()/2;  
    int center_y = img.TellHeight()/2;  

    //temp variables to store shift origin values
    circle tc1 = c1;
    circle tc2 = c2;

    auto shiftOrigin = [&](int &x,int &y){
        x = center_x - x;
        y = center_y - y;
    };

    //not required 
    shiftOrigin(tc1.center_x,tc1.center_y);
    shiftOrigin(tc2.center_x,tc2.center_y);
    shiftOrigin(center_x,center_y);
   
    double m1 = (tc1.center_y - tc2.center_y)*1.0/
                  (tc1.center_x - tc2.center_x);

    //slope of line joining the center of a circle and the center of the image
    double m2 = (tc1.center_y - center_y)*1.0/
                    (tc1.center_x - center_x);
    
    double angle = 90 - round(atan (m1) * 180 / PI);
    
    //distance between center of one circle and the center of the image
    double pDist = dist(tc1.center_x,tc1.center_y,center_x,center_y);
    
    double diff1 = atan(m1)-atan(m2);
    // float diff2 = m1-m2;

    pDist = round(pDist * sin(diff1));
    
    if(tan(diff1) > 1)
        pDist = -pDist;

    const double thetaTHRES = 10;
    const double radTHRES = 10;

    for(auto &tline : linesVector){
        if( inRange((double)tline.theta,angle,thetaTHRES) && 
                inRange((double)tline.rad,pDist,radTHRES))
            return true;
    }
    // std::cout<<pDist<<"\t"<<angle<<"\n";
    return false;
}



void checkEdges(BMP &img,const std::list<circle> &circleVector,const std::list<line> &linesVector){

    const int dim = circleVector.size();
    bool adjMat[dim][dim];
    int result=0;
    
    int k,l;
    std::list<circle>::const_iterator i,j;

    /*this particular code is because of list nature of the containers and
    also to increase the efficincy of code by not calling the edgeExist function twice 
    for same pair of circles*/

    for(i = circleVector.cbegin(),k = 0;i != circleVector.cend(); ++i,++k){
        for(j = circleVector.cbegin(),l = 0;j != circleVector.cend(); ++j,++l){

            if(k == l){
                result = 0;
            }
            else if( l > k){
                result = edgeExist(img,*i,*j,linesVector);
            }else{
                result = adjMat[l][k];    
            }
            adjMat[k][l] = result;
            std::cout<<result<<"\t";
        }
        std::cout<<"\n";
    }
}

//Convert Line to Line segments
void lineToLineSegment(BMP &img,std::list<line> &linesVector,
                                std::vector<lineSeg> &lsv){
    BMP origImg(img);
    DFOR(i,j,img.TellWidth(),img.TellHeight()){
        SetPixel(img(i,j),0);
    }

    for(auto &i:linesVector){
        auto t = polarToCoord(img,i.rad,i.theta);
        DrawlineSegment(origImg,img,std::get<0>(t),std::get<1>(t),
                            std::get<2>(t),std::get<3>(t),255);
    }
    // connectedComponent(img,lsv);
    // std::cout<<lsv.size()<<"\n";
}

bool findLineSeg(circle &c1,circle &c2,std::vector<lineSeg> &lsv){
    const int BUFF = 5;

    for(auto &i:lsv){
        if ((dist(c1.center_x,c1.center_y,i.x1,i.y1) < c1.rad + BUFF &&
                dist(c2.center_x,c2.center_y,i.x2,i.y2) < c2.rad + BUFF) ||
            (dist(c2.center_x,c2.center_y,i.x1,i.y1) < c2.rad + BUFF &&
                dist(c1.center_x,c1.center_y,i.x2,i.y2) < c1.rad + BUFF)
        )
        return true;
    }
    return false;
}


void checkEdgesLineSeg(std::list<circle> &circleVector,
                            std::vector<lineSeg> &lsv){
 
    const int dim = circleVector.size();
    bool adjMat[dim][dim];
    int result=0;
    
    int k,l;
    std::list<circle>::iterator i,j;

    for(i = circleVector.begin(),k = 0;i != circleVector.end(); ++i,++k){
        for(j = circleVector.begin(),l = 0;j != circleVector.end(); ++j,++l){

            if(k == l)
                result = -1;
            else if( l > k)
                result = findLineSeg(*i,*j,lsv);
            else
                 result = adjMat[l][k];    
    
            adjMat[k][l] = result;
            std::cout<<result<<"\t";
        }
        std::cout<<"\n";
    }
}

bool findLineSeg2(circle &c1,circle &c2,std::list<line> &linesVector){
    const int THRES_THETA= 5;

    // const int center_x = img.TellWidth()/2;  
    // const int center_y = img.TellHeight()/2;  

    // auto shiftOrigin = [&](int &x,int &y){
    //     x = center_x - x;
    //     y = center_y - y;
    // };

    // circle tc1 = c1;
    // circle tc2 = c2;

    // shiftOrigin(tc1.center_x,tc1.center_y);
    // shiftOrigin(tc2.center_x,tc2.center_y);

    double m1 = (c1.center_y - c2.center_y)*1.0/
                  (c1.center_x - c2.center_x);
    
    double angle = atan(m1) * 180 / PI;


    int mid_x = (c1.center_x + c2.center_x)/2;
    int mid_y = (c1.center_y + c2.center_y)/2;

    dbg("angle: %f",round(angle));

    // std::cout<<"\t"<<round(angle)<<"\n";
    // for(auto &i:linesVector){
    //     if()
    //         return true;
    // }
    return false;
}

void method3(std::list<circle> &circleVector,
                            std::list<line> &linesVector){
    
    const int dim = circleVector.size();
    bool adjMat[dim][dim];
    int result=0;
    
    int k,l;
    std::list<circle>::iterator i,j;


    for(i = circleVector.begin(),k = 0;i != circleVector.end(); ++i,++k){
        for(j = circleVector.begin(),l = 0;j != circleVector.end(); ++j,++l){

            if(k == l)
                result = 0;
            else if( l > k)
                result = findLineSeg2(*i,*j,linesVector);
            else
                 result = adjMat[l][k];    
    
            adjMat[k][l] = result;
            // std::cout<<result<<"\t";
        }
        // std::cout<<"\n";
    }
}

void drawLinesAndCircles(BMP &img,const std::list<circle> &circleVector,const std::list<line> &linesVector){

    for(auto &i:circleVector){
        // DrawCircle(img,i.center_x,i.center_y,i.rad,0,255);
    }
    
    for(auto &i:linesVector){
        auto t = polarToCoord(img,i.rad,i.theta);
        Drawline(img,std::get<0>(t),std::get<1>(t),
                            std::get<2>(t),std::get<3>(t),255);
    }
}

void imgToAdjGraph(BMP &img){
    const int THRES1 = 120;
    
    BMP origImg(img);
    // RGBtoHSI(origImg);
    thresholding(origImg,THRES1);
    // sobelOperator(origImg); 

    std::list<circle> circleVector;
    HughTransformCircle(origImg,img,circleVector);
    filterCircles(circleVector);
    
    std::list<line> linesVector;
    HughTransformLine(origImg,img,linesVector);
    filterLines(linesVector);    

    std::cout<<"circles:\n";
    for(auto &i:circleVector){
        i.print();
    }

    std::cout<<"\nlines:\n";
    for(auto &i:linesVector){
        i.print();
    }

    for(auto &i:linesVector){
        auto t = polarToCoord(img,i.rad,i.theta);
        i = {i.rad,i.theta,std::get<0>(t),std::get<1>(t),std::get<2>(t),std::get<3>(t)};
    }

    // connectedComponent

    // method3(circleVector,linesVector);

    // std::vector<lineSeg> lineSegVector;
    // lineToLineSegment(img,linesVector,lineSegVector);

    // std::cout<<"line Segments:\n";
    // for(auto &i:lineSegVector){
    //     i.print();
    // }

    // checkEdgesLineSeg(circleVector,lineSegVector);

    // std::cout<<"\nslope of connections:\n";
    // std::cout<<"dist\tangle\n\n";

    drawLinesAndCircles(img,circleVector,linesVector);
    std::cout<<"\nadjacency matrix:\n";
    checkEdges(img,circleVector,linesVector);

    
    // connectedComponent(img);
    
    // const int adjMatDim = circleVector.size();
    // int adjMat[adjMatDim][adjMatDim];

    // std::cout<<"lines: "<<linesVector.size()<<"\n";
    // std::cout<<"circles: "<<circleVector.size()<<"\n";
}



void (*fun_arr[]) (BMP &) = {
/*1*/   imgToAdjGraph
// /*8*/   ,HughTransformLine    
// /*9*/   ,HughTransformCircle
#if 0
/*10*/  ,testLine
/*11*/  ,testCircle
#endif
};

#define arrSize(var)  (int)(sizeof(var) / sizeof(*(var)))

int main(int argc, char* argv[]){

    if(argc < 3 || 
        atoi(argv[1]) < 0 || 
            atoi(argv[1]) >= arrSize(fun_arr) )
    {
        std::cerr<<"incorrect args\n";
        std::cerr<<"usage: "<<argv[0]<<" <operation to perform 0-"<<arrSize(fun_arr)-1<<"> <input_file> [output_file]\n";
        std::cerr<<"please check README file for list of available operations\n";
        return -1;
    }

    // qBIa();

    BMP AnImage;
    AnImage.ReadFromFile(argv[2]);
    
    BMP Output(AnImage);
    fun_arr[atoi(argv[1])](Output);
    
    if(argc == 4 && strlen(argv[3]) != 0)
        Output.WriteToFile(argv[3]);
    else
        Output.WriteToFile("Output1.bmp");

    // printImg(Output);

    return 0;
}
