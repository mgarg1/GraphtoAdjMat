#include "../myUtilities/myUtilities.hpp"
#include "../templateMatch/templateMatch.hpp"


CI thetaTHRES = 10;
CI radTHRES = 10;
CI minLinePixelIntensityTHRES = 250;
CI minLineAccuTHRES = 80;

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

std::tuple<int,int,int,int> 
polarToCoord(const BMP &img,const int rad,const double theta){
    
    const int img_w = img.TellWidth();
    const int img_h = img.TellHeight();
    const int center_x = img_w/2;
    const int center_y = img_h/2;
    //converting to radians
    const double thetaRad = theta * b2RAD;
    const auto sinValue = sin(thetaRad);
    const auto cosValue = cos(thetaRad);

    int x1,y1,x2,y2;
    std::tuple<int,int,int,int> t1;  
    if(theta >= 45 && theta <= 135){ 
        double ax,bx;

        //start to end
        y1 = 0; 
        y2 = img_h;

        //x = (rad - y sin(theta)) / cos(theta);  
        ax = (rad - (y1 - center_y ) * cosValue) / sinValue;  
        bx = (rad - (y2 - center_y ) * cosValue) / sinValue;  
        
        //apply offsets
        x1 = round(ax + center_x); 
        x2 = round(bx + center_x);
    }  
    else{
        double ay,by;

        //start to end
        x1 = 0; 
        x2 = img_w; 

        ay = (rad - (x1 - center_x) * sinValue ) / cosValue;
        by = (rad - (x2 - center_x) * sinValue ) / cosValue;

        //apply offsets
        y1 = round(ay + center_y); 
        y2 = round(by + center_y);

    }
    // std::cout<<theta<<"\t"<<rad<<"\t";
    // std::cout<<x1<<","<<y1<<" to "<<x2<<","<<y2<<std::endl;
    return std::make_tuple(x1,y1,x2,y2);
    // DrawlineSegment(img,x1,y1,x2,y2,RED);
}

struct line{
    int rad;
    int theta;
    
    int x1,y1,x2,y2;

    void print(){
        dbg("r:%d th:%d : (%d,%d) to (%d,%d)",rad,theta,x1,y1,x2,y2);
        // std::cout<<rad<<"\t"<<theta<<"\n";
    }
};

struct circle{
    char name;
    int rad;
    int center_x;
    int center_y;

    circle(int r,int cx,int cy):rad(r),center_x(cx),center_y(cy){}
    
    void setName(char ch){
    	name = ch;
    }

    void print(){
        std::cout<<rad<<"\t"<<center_x<<"\t"<<center_y<<"\n";
    }
};


void HughTransformLine(const BMP &origImg,BMP &img,std::list<line> &linesVector){ 

    RGBtoHSI(img);
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
        if(origImg.GetPixel(x,y).Green > minLinePixelIntensityTHRES){  
            for(int theta=0;theta<180;theta++){  
              //i = xsin(theta) + ycos(theta);
                auto cosValue = cos(theta * b2RAD);
                auto sinValue = sin(theta * b2RAD);

                //here theta is the angle between the x-axis and the line perpendicular to given line  
                //and radius is perpendicular distance of line from origin
                int rad = round( (center_x - x) * cosValue + (center_y - y) * sinValue );  
                // i + max_r to make everything positive
                assert(max_r+rad > 0,max_r + rad);
                accumulator[max_r + rad][theta]++;  
            }  
        }      
    }

    const int DIM = 9; 
    int slope;

    //Search if the point is a local Maxima in accumulator
    DFOR(rad,theta,_accu_x,_accu_y){
        if(accumulator[rad][theta] > minLineAccuTHRES){
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
            
            //we are now interested in slope of line
            slope = theta;
            if(theta >= 0 && theta < 90 )
                slope += 90;
            else if(theta >= 90 && theta <= 180)
                slope -= 90;

            linesVector.push_back(line{rad-max_r,slope});
            label1:;
        }
    }
    // identifyLines(img,radii,thetas);
}

void DrawCircle(BMP &img,const int x0, const int y0, const int radius,
                    const int rVal,const int gVal=0,const int bVal=0)
{
  int x = radius;
  int y = 0;
  int radiusError = 1-x;
  // int rVal=255,gVal=0,bVal=0;

  while(x >= y)
  {
    
    SetPixelSizeSafe(img,x+x0,y+y0,rVal,gVal,bVal);
    SetPixelSizeSafe(img,x+x0,-y+y0,rVal,gVal,bVal);
    SetPixelSizeSafe(img,-x+x0,y+y0,rVal,gVal,bVal);
    SetPixelSizeSafe(img,-x+x0,-y+y0,rVal,gVal,bVal);
    
    SetPixelSizeSafe(img,y+x0,x+y0,rVal,gVal,bVal);
    SetPixelSizeSafe(img,-y+x0,x+y0,rVal,gVal,bVal);
    SetPixelSizeSafe(img,y+x0,-x+y0,rVal,gVal,bVal);
    SetPixelSizeSafe(img,-y+x0,-x+y0,rVal,gVal,bVal);
    y++;

    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x) + 1;
    }
  }
}

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

/* this function check whether there exists a line between two given circles*/
bool edgeExist(const BMP &img,const circle &c1,const circle &c2,const std::list<line> &linesVector){
    
    int center_x = img.TellWidth()/2;  
    int center_y = img.TellHeight()/2;  

    //temp variables to store shift origin values
    // circle tc1 = c1;
    // circle tc2 = c2;

    // auto shiftOrigin = [&](int &x,int &y){
    //     x = center_x - x;
    //     y = center_y - y;
    // };

    // //not required 
    // shiftOrigin(tc1.center_x,tc1.center_y);
    // shiftOrigin(tc2.center_x,tc2.center_y);
    // shiftOrigin(center_x,center_y);
   
   //slope of line joining the center of two circles
    double m1 = (c1.center_y - c2.center_y)*1.0/
                  (c1.center_x - c2.center_x);

    //slope of line joining the center of a circle and the center of the image
    double m2 = (c1.center_y - center_y)*1.0/
                    (c1.center_x - center_x);
    
    int angle = round(atan (m1) * 180 / PI);
    
    //distance between center of one circle and the center of the image
    double pDist = dist(c1.center_x,c1.center_y,center_x,center_y);
    
    double diff1 = atan(m1)-atan(m2);
    // float diff2 = m1-m2;

    pDist = round(pDist * sin(diff1));
    
    if(tan(diff1) > 1)
        pDist = -pDist;
    if(angle < 0)
        angle = 180 + angle;

    // dbg("r:%2f th:%2d -- c1:%c to c2:%c",pDist,angle,c1.name,c2.name);
    

    auto thetaCheck = [](CI t1,CI t2){
        // int lim1,lim2,val;

        if(t2-thetaTHRES < 0){
            if ( (t1 >= 0 && t1 < t2+thetaTHRES) || ( t1 <= 180 &&  t1 > ( (t2-thetaTHRES)%180) ) )
                return true;    
        }
        else if(t2+thetaTHRES > 180){
            if ( (t1 >= 0 && (t1 < (t2+thetaTHRES)%180)  ) || ( t1 <= 180 && t1 > t2-thetaTHRES) )
                return true; 
        }
        else{
            if ( (t2-thetaTHRES < t1) && (t2+thetaTHRES > t1) )
                return true; 
        } 
        return false;
    };

    for(auto &tline : linesVector){
        if( thetaCheck(angle,tline.theta) && 
                inRange((double)tline.rad,pDist,(double)radTHRES))
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
    std::cout<<"\t"; 
    for(auto &i:circleVector){
    	std::cout<<i.name<<"\t";
    }
    std::cout<<"\n";

    for(i = circleVector.cbegin(),k = 0;i != circleVector.cend(); ++i,++k){
        for(j = circleVector.cbegin(),l = 0;j != circleVector.cend(); ++j,++l){
        	if(l==0){
        		std::cout<<(*i).name<<"\t";
        	}
            if(k == l){
            	std::cout<<"-\t";
                continue;
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

void FloodFill3(BMP &img,CI i,CI j,int *grp_no,CI group){

    const int h = img.TellHeight();
    const int w = img.TellWidth();
    const RGBApixel curPixel = *img(i,j);

    // assert(sizeof(grp_no)==4*w,sizeof(grp_no));

    std::function<void (CI,CI)> intRec = [&](CI x,CI y){
      
        if(x >= w-1 || x < 0 || y >= h-1 || y < 0 ||
            img.GetPixel(x,y) != curPixel || grp_no[x*h+y] != 0)
            return;

        // SetPixel(img(x,y),255,0,0);
        // pattern.offset.push_back({pattern.startX-x,pattern.startY-y});
        grp_no[x*h+y] = group;

        intRec(x+1,y);
        intRec(x,y+1);
        intRec(x,y-1);
        intRec(x-1,y);

        intRec(x+1,y+1);
        intRec(x+1,y-1);
        intRec(x-1,y+1);
        intRec(x-1,y-1);
    };
    intRec(i,j);
}

void maxEliminate(BMP &img){

    const int w = img.TellWidth();
    const int h = img.TellHeight();

    int groups = 1;

    // int grp_no[w][h];
    // std::array<std::array<int,h>,w> grp_no; 
    int *grp_no;
	int *grp_cnt = NULL;
    
    grp_no = new int[w*h];

    //allocating 2-d arrays
    // grp_no = new int*[w]; 
    // for (int i = 0; i < w; ++i){
    //     grp_no[i] = new int[h]; 
    //     memset(grp_no[i],0,h*sizeof(int));
    // }

    //asuming img(0,0)'s pixel is background
    RGBApixel temp = *img(0,0);
    // SetPixel(&temp,0);
    
    // dbg("%lu",sizeof(*grp_no));

    DFOR(i,j,w,h){
        if(*img(i,j) != temp && (grp_no[i*h+j] == 0)){
            FloodFill3(img,i,j,grp_no,groups);
            groups++;
        }
    }
    
    dbg("groups : %d",groups);
   
    grp_cnt = new int[groups];
    
    DFOR(i,j,w,h){
        grp_cnt[grp_no[i*h+j]]++;
    }

    // connectedComponent(img,grp_no,grp_cnt);

    //index 0 belongs to background
    int maxIndex = 1;
    for(int i=2;i<groups;++i){
    	if(grp_cnt[i] > grp_cnt[maxIndex])
    		maxIndex = i;
    }

    // auto result = std::max_element(grp_cnt,grp_cnt + sizeof(grp_cnt));
    
    dbg("index : %d",maxIndex);
    dbg("max Value : %d",grp_cnt[maxIndex]);
 

    DFOR(i,j,w,h){
        if(grp_no[i*h+j] == maxIndex){
        	SetPixel(img(i,j),&temp);
        }
    }     

    // for (int i = 0; i < w; ++i){
    //     delete[] grp_no[i];
    // }
    delete[] grp_no;
    delete[] grp_cnt;
}

void drawLinesAndCircles(BMP &img,const std::list<circle> &circleVector,const std::list<line> &linesVector){

    for(auto &i:circleVector){
        DrawCircle(img,i.center_x,i.center_y,i.rad,0,255);
    }
    
    for(auto &i:linesVector){
        auto t = polarToCoord(img,i.rad,i.theta);
        Drawline(img,std::get<0>(t),std::get<1>(t),
                            std::get<2>(t),std::get<3>(t),255);
    }
}

char imgArr[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D'};

void nameVertices(BMP &img,std::list<circle> &circleVector){
	BMP inImg(img);
	// maxEliminate(inImg);
	dataBase db1(".\\templateMatch\\DB\\",imgArr,arrSize(imgArr));

	for(auto &i:circleVector){
		/*Yet to Correct*/
		char ch = db1.identifySingleCharAt(inImg,i.center_x-(i.rad/2),i.center_y-(i.rad/2),i.rad,i.rad);
		i.setName(ch);
		dbg("centerx: %d, centery: %d,char ::%c",i.center_x,i.center_y,ch);
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
    
    // std::cout<<"circles:\n";
    // for(auto &i:circleVector){ i.print(); }

    std::list<line> linesVector;
    HughTransformLine(origImg,img,linesVector);
    filterLines(linesVector);    

    for(auto &i:linesVector){
        auto t = polarToCoord(img,i.rad,i.theta);
        i = {i.rad,i.theta,std::get<0>(t),std::get<1>(t),std::get<2>(t),std::get<3>(t)};
    }

    std::cout<<"\nlines:\n";
    for(auto &i:linesVector){ i.print(); }

    // drawLinesAndCircles(img,circleVector,linesVector);
    std::cout<<"\nadjacency matrix:\n";

    nameVertices(img,circleVector);
    checkEdges(img,circleVector,linesVector);
    
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
