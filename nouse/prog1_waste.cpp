#include "./myUtilities/myUtilities.hpp"

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


// template<class T>
// void SetPixel2(const RGBApixel refpi,RGBApixel *p1,T b1,T b2,T b3){
//     if(refpi.Green > 250)
//         SetPixel(p1,b1,b2,b3);
// }

// void DrawlineSegment(const BMP &origImg,BMP &img,int x1,int y1,int x2,int y2,int color){
     
//     auto chk_x = [](int &coord,const int lim){
//         coord = coord >= 0 ? coord :0; 
//         coord = coord <= lim-1 ? coord :lim-1;
//     };

//     chk_x(x1,img.TellWidth());
//     chk_x(y1,img.TellHeight());
//     chk_x(x2,img.TellWidth());
//     chk_x(y2,img.TellHeight());

//     int delta_x = x2 - x1;
//     int delta_y = y2 - y1;
    
//     char const ix = (delta_x > 0) - (delta_x < 0);
//     char const iy = (delta_y > 0) - (delta_y < 0);
    
//     delta_x = std::abs(delta_x) << 1;
//     delta_y = std::abs(delta_y) << 1;
 
//     SetPixel2(origImg.GetPixel(x1,y1),img(x1,y1),0,0,color);
//     // plot(x1, y1);
 
//     if (delta_x >= delta_y){
//         // error may go below zero
//         int error(delta_y - (delta_x >> 1));
 
//         while (x1 != x2){
//             if ((error >= 0) && (error || (ix > 0))){
//                 error -= delta_x;
//                 y1 += iy;
//             }
//             // else do nothing
//             error += delta_y;
//             x1 += ix;
//             SetPixel2(origImg.GetPixel(x1,y1),img(x1,y1),color,0,0);
//         }
//     }
//     else{
//         // error may go below zero
//         int error(delta_x - (delta_y >> 1));
 
//         while (y1 != y2){
//             if ((error >= 0) && (error || (iy > 0))){
//                 error -= delta_y;
//                 x1 += ix;
//             }
//             // else do nothing
//             error += delta_x;
//             y1 += iy;
//             SetPixel2(origImg.GetPixel(x1,y1),img(x1,y1),color,0,0);
//         }
//     } 
// }

// #define test_p(x,y) ( (x) >= img.TellWidth()-1  ) || ( (x) < 0 ) || \
//                     ( (y) >= img.TellHeight()-1 ) || ( (y) < 0 ) || \
//                      (img.GetPixel((x),(y)) != cur_pixel) 
                    

// std::pair<int,int> minX,maxX,minY,maxY;


// /*this FloodFill algorithm return true if it is a valid distinct region*/
// void FloodFill2(BMP &img,const int i,const int j,
//                 std::vector<RGBApixel *> &v,const RGBApixel &cur_pixel){

//     if(test_p(i,j)){
//         return;
//     }
//     SetPixel(img(i,j),0,255,0);
//     v.push_back(img(i,j));    

//     // grp_no[i][j] = group;
//     // const ebmpBYTE temp = img.GetPixel(i,j).Green
//     //return 
//     if(i < minX.first)
//         minX = {i,j};
//     if(i > maxX.first)
//         maxX = {i,j};
//     if(j < minY.second)
//         minY = {i,j};
//     if(j > maxY.second)
//         maxY = {i,j};


//     FloodFill2(img,i+1,j,v,cur_pixel);
//     FloodFill2(img,i,j+1,v,cur_pixel);
//     FloodFill2(img,i,j-1,v,cur_pixel);
//     FloodFill2(img,i-1,j,v,cur_pixel);
    
//     FloodFill2(img,i+1,j+1,v,cur_pixel);
//     FloodFill2(img,i+1,j-1,v,cur_pixel);
//     FloodFill2(img,i-1,j+1,v,cur_pixel);
//     FloodFill2(img,i-1,j-1,v,cur_pixel);
    
//     // FloodFill2(img,i,j-1,v,cur_pixel);
//     // FloodFill2(img,i-1,j,v,cur_pixel);
// }

// struct lineSeg{
//     int x1,y1,x2,y2;
//     void print(){
//         printf("Line Segment : %d,%d to %d,%d\n",x1,y1,x2,y2);
//     }
// };


// void connectedComponent(BMP &img,std::vector<lineSeg> &lsv){
    
//     const int THRES = 10;
//     int groups = 1;
//     const int width = img.TellWidth();
//     const int height = img.TellHeight();
//     std::vector<RGBApixel *> v;
//     // std::cout<<"in here\n";
//     DFOR(i,j,width,height){
//         if(img(i,j)->Red != 0){
//             // std::cout<<"found one grp\n";
//             const RGBApixel temp = img.GetPixel(i,j);
//             minX = {100000,11};
//             maxX = {-100000,11};
//             minY = {11,100000};
//             maxY = {11,-100000};

//             FloodFill2(img,i,j,v,temp);
//             // std::cout<<v.size()<<" "<<i<<" "<<j<<"\n";
            
//             if(v.size() < THRES){
//                 //paint black
//                 for(auto temp_pixel:v){
//                     SetPixel(temp_pixel,0);
//                 }
//             }else if(minX.first != maxX.first){
//                 //both minX and maxX will be the end-points
//                 lsv.push_back(lineSeg{minX.first,minX.second,maxX.first,maxX.second});
//             }
//             else{
//                 //think of other end-points
//                 lsv.push_back(lineSeg{minY.first,minY.second,maxY.first,maxY.second});
//             }

//             v.clear();
//         }
//     }
// }

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

//Convert Line to Line segments
// void lineToLineSegment(BMP &img,std::list<line> &linesVector,
//                                 std::vector<lineSeg> &lsv){
//     BMP origImg(img);
//     DFOR(i,j,img.TellWidth(),img.TellHeight()){
//         SetPixel(img(i,j),0);
//     }

//     for(auto &i:linesVector){
//         auto t = polarToCoord(img,i.rad,i.theta);
//         DrawlineSegment(origImg,img,std::get<0>(t),std::get<1>(t),
//                             std::get<2>(t),std::get<3>(t),255);
//     }
//     // connectedComponent(img,lsv);
//     // std::cout<<lsv.size()<<"\n";
// }

// bool findLineSeg(circle &c1,circle &c2,std::vector<lineSeg> &lsv){
//     const int BUFF = 5;

//     for(auto &i:lsv){
//         if ((dist(c1.center_x,c1.center_y,i.x1,i.y1) < c1.rad + BUFF &&
//                 dist(c2.center_x,c2.center_y,i.x2,i.y2) < c2.rad + BUFF) ||
//             (dist(c2.center_x,c2.center_y,i.x1,i.y1) < c2.rad + BUFF &&
//                 dist(c1.center_x,c1.center_y,i.x2,i.y2) < c1.rad + BUFF)
//         )
//         return true;
//     }
//     return false;
// }


// void checkEdgesLineSeg(std::list<circle> &circleVector,
//                             std::vector<lineSeg> &lsv){
 
//     const int dim = circleVector.size();
//     bool adjMat[dim][dim];
//     int result=0;
    
//     int k,l;
//     std::list<circle>::iterator i,j;

//     for(i = circleVector.begin(),k = 0;i != circleVector.end(); ++i,++k){
//         for(j = circleVector.begin(),l = 0;j != circleVector.end(); ++j,++l){

//             if(k == l)
//                 result = -1;
//             else if( l > k)
//                 result = findLineSeg(*i,*j,lsv);
//             else
//                  result = adjMat[l][k];    
    
//             adjMat[k][l] = result;
//             std::cout<<result<<"\t";
//         }
//         std::cout<<"\n";
//     }
// }

// bool findLineSeg2(circle &c1,circle &c2,std::list<line> &linesVector){
//     const int THRES_THETA= 5;

//     // const int center_x = img.TellWidth()/2;  
//     // const int center_y = img.TellHeight()/2;  

//     // auto shiftOrigin = [&](int &x,int &y){
//     //     x = center_x - x;
//     //     y = center_y - y;
//     // };

//     // circle tc1 = c1;
//     // circle tc2 = c2;

//     // shiftOrigin(tc1.center_x,tc1.center_y);
//     // shiftOrigin(tc2.center_x,tc2.center_y);

//     double m1 = (c1.center_y - c2.center_y)*1.0/
//                   (c1.center_x - c2.center_x);
    
//     double angle = atan(m1) * 180 / PI;


//     int mid_x = (c1.center_x + c2.center_x)/2;
//     int mid_y = (c1.center_y + c2.center_y)/2;

//     dbg("angle: %f",round(angle));

//     // std::cout<<"\t"<<round(angle)<<"\n";
//     // for(auto &i:linesVector){
//     //     if()
//     //         return true;
//     // }
//     return false;
// }

// void method3(std::list<circle> &circleVector,
//                             std::list<line> &linesVector){
    
//     const int dim = circleVector.size();
//     bool adjMat[dim][dim];
//     int result=0;
    
//     int k,l;
//     std::list<circle>::iterator i,j;


//     for(i = circleVector.begin(),k = 0;i != circleVector.end(); ++i,++k){
//         for(j = circleVector.begin(),l = 0;j != circleVector.end(); ++j,++l){

//             if(k == l)
//                 result = 0;
//             else if( l > k)
//                 result = findLineSeg2(*i,*j,linesVector);
//             else
//                  result = adjMat[l][k];    
    
//             adjMat[k][l] = result;
//             // std::cout<<result<<"\t";
//         }
//         // std::cout<<"\n";
//     }
// }

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

    // drawLinesAndCircles(img,circleVector,linesVector);
    std::cout<<"\nadjacency matrix:\n";
    checkEdges(img,circleVector,linesVector);
    maxEliminate(img);
    
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
