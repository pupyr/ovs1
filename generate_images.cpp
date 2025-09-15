#include <stdlib.h>
#include <map>
#include <iostream>
#include <sstream>
#include "bmp.cpp"
#include "figure.cpp"
using namespace std;

void drawFigure(bmp* b, figure fig, int x, int y){
    for(int s = 0; s<fig.size; s++)
        if(x+fig.coords[s].x<7 && y+fig.coords[s].y<7){
            b->set(x+fig.coords[s].x,y+fig.coords[s].y,1);
        }
}

void save(string filename, bmp b)
{
    FILE * fp;
    char *c;
    int size = b.size();
  
    fp = fopen((char*) filename.c_str(), "wb");
    c = (char*) &b;
    for (int i = 0; i < size; i++)
        putc(*c++, fp);
    fclose(fp);
}

void makeImages(){
    for(pair<string, figure> p: Figure)
        for(int i=0; i<=7-p.second.width; i++)
            for(int j=0; j<=7-p.second.height; j++){
                bmp BMP = bmp((char*)"imgs/img.bmp");
                drawFigure(&BMP, p.second, i, j);
                stringstream ss;
                ss << "imgs/" << p.first << "/" << i << "_" << j << ".bmp";
                string s = ss.str();
                save(s, BMP);
            }
}

int main(){
    makeImages();
}