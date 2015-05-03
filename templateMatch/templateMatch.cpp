#include "templateMatch.hpp"

dbImg::dbImg(std::string fileLoc,char ch)
            :val(ch){

    intImg.ReadFromFile(fileLoc.c_str());
    dbg("success : open file %s",fileLoc.c_str());
    thresholding(intImg,THRES_LIMIT);
    // intImg.WriteToFile(fileLoc.c_str());

    endX = intImg.TellWidth();
    endY = intImg.TellHeight();

    DFOR(i,j,endX,endY){
        if(intImg(i,j)->Green > 0){
            offX = i;
            offY = j;
            // dbg("%d %d",offX,offY);
            // FloodFill2(img,i,j,*this,*img(i,j));
            return;
        }
    }
}

dbImg::dbImg(const dbImg &img2)
    :val(img2.val),offX(img2.offX),offY(img2.offY),
    endX(img2.endX),endY(img2.endY){
        intImg = img2.intImg;
}

int dbImg::compare(const BMP &img,const int x,const int y) const{
    int cnt = 0;
    const int w = img.TellWidth();
    const int h = img.TellHeight();

    // dbg("%d %d %d %d",endX,endY,offX,offY);

    //assuming that the image will always be greater than the database image
    if(x+endX-offX > w || y+endY-offY > h){
        return 0;
    }

    DFOR(i,j,endX-offX,endY-offY){
        if(img.GetPixel(x+i,y+j).Green != 0){
            if(img.GetPixel(x+i,y+j) == intImg.GetPixel(offX+i,offY+j)){
                    cnt++;
            }
        }
    }
    return cnt;
}

dataBase::dataBase(std::string baseAddr,char *arr,size_t sz){
    for(size_t i=0;i<sz;++i){
        std::stringstream ss;
        ss<<baseAddr<<arr[i]<<".bmp";
        insert(ss.str(),arr[i]);
        // dbg("opened: %d",i);
    }
}

void dataBase::insert(std::string fileLoc,char ch){
    v.push_back(new dbImg(fileLoc,ch));
}

char dataBase::identifySingleCharAt(BMP &img,CI startX,CI startY,CI lenX,CI lenY) const{
    CI h = img.TellHeight();
    CI w = img.TellWidth();

    auto findMatchingIndex = [&](int startX,int startY){
        std::vector<int> errorTable(v.size());
        int maxIndex = 0;

        for(size_t i=0;i<v.size();++i){
            errorTable[i] = v[i]->compare(img,startX,startY);
            
            if(errorTable[maxIndex] < errorTable[i])
                maxIndex = i;

            // dbg("%d",errorTable[i]);
        }
        return maxIndex;
    };      

    //assuming img(0,0) as background
    DFOR(i,j,lenX,lenY){
        if(*img(i+startX,j+startY) != *img(0,0)){
            return v[findMatchingIndex(i+startX,j+startY)]->val;
        }
    }
    //error
    return '\0';
}


#if 0

void FloodFill2(BMP &img,const int i,const int j/*,dbImg &pattern*/,const RGBApixel curPixel,const RGBApixel destPixel){

    const int h = img.TellHeight();
    const int w = img.TellWidth();
    // pattern.startX = i;
    // pattern.startY = j;

    std::function<void (int,int)> intRec = [&](int x,int y){
      
        if(x >= w-1 || x < 0 || y >= h-1 || y < 0 ||
            img.GetPixel(x,y) != curPixel)
            return;

        SetPixel(img(x,y),255,0,0);
        // pattern.offset.push_back({pattern.startX-x,pattern.startY-y});

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

void openFiles(std::string baseAddr){

    for(int i=0;i<DIGITS;++i){
        std::stringstream ss;
        ss<<i<<".bmp";
        db[i] = new dbImg(baseAddr + ss.str() , '0' + i);
        dbg("opened: %d",i);
    }
}

void findNums(BMP &img){
    const int h = img.TellHeight();
    const int w = img.TellWidth();

    auto findMatchingIndex = [&](int startX,int startY){
        int errorTable[DIGITS];

        for(int i=0;i<DIGITS;++i){
            errorTable[i] =   (db[i])->compare(img,startX,startY);
        }

        int maxIndex = 0;
        for(int i=1;i<DIGITS;++i){
           if(errorTable[maxIndex] < errorTable[i]){
                maxIndex = i;
           }
        }

        for(int i=0;i<DIGITS;++i){
            dbg("%d",errorTable[i]);
        }

        return maxIndex;
    };

    DFOR(i,j,w,h){
        if(img(i,j)->Green > 100){
            std::cout<<findMatchingIndex(i,j)<<"\n";
            // FloodFill2(img,i,const int j/*,dbImg &pattern*/,const RGBApixel curPixel,const RGBApixel destPixel){
            FloodFill2(img,i,j,*img(i,j),*img(i,j));
            // return;
        }
    }
}

int main(int argc, char* argv[]){

    // if(argc < 3 || 
    //     atoi(argv[1]) < 0 || 
    //         atoi(argv[1]) >= arrSize(fun_arr) )
    // {
    //     std::cerr<<"incorrect args\n";
    //     std::cerr<<"usage: "<<argv[0]<<" <operation to perform 0-"<<arrSize(fun_arr)-1<<"> <input_file> [output_file]\n";
    //     std::cerr<<"please check README file for list of available operations\n";
    //     return -1;
    // }
    openFiles(".\\DB\\");

    BMP AnImage;
    AnImage.ReadFromFile("input2.bmp");
    thresholding(AnImage,THRES_LIMIT);
        
    findNums(AnImage);
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

#endif