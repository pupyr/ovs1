#include <stdio.h>
#include <stdlib.h>
#include <map>
using namespace std;

struct point{
    char p[3];
};

struct bmp{
    char header[54];
    point data[7][8];
};

void set(struct bmp *b, int i, int j, int value){
    b->data[6-j][i] = value==0 
            ? (point){-1,-1,-1} 
            : (point){ 0, 0, 0};
}

int get(struct bmp *b, int i, int j){
    return b->data[6-i][j].p[0] == 0;
}

struct coord{
    int x,y;
};

coord triangle[14] = {
    (coord){3,0},
    (coord){4,1},
    (coord){5,2},
    (coord){6,3},
    (coord){6,4},
    (coord){5,4},
    (coord){4,4},
    (coord){3,4},
    (coord){2,4},
    (coord){1,4},
    (coord){0,4},
    (coord){0,3},
    (coord){1,2},
    (coord){2,1}
};

coord square[16] = {
    (coord){0,0},
    (coord){0,1},
    (coord){0,2},
    (coord){0,3},
    (coord){0,4},
    (coord){1,4},
    (coord){2,4},
    (coord){3,4},
    (coord){4,4},
    (coord){4,3},
    (coord){4,2},
    (coord){4,1},
    (coord){4,0},
    (coord){3,0},
    (coord){2,0},
    (coord){1,0}
};

coord circle[12] = {
    (coord){0,1},
    (coord){0,2},
    (coord){0,3},
    (coord){1,4},
    (coord){2,4},
    (coord){3,4},
    (coord){4,3},
    (coord){4,2},
    (coord){4,1},
    (coord){3,0},
    (coord){2,0},
    (coord){1,0}
};

struct figure{
    coord* coords;
    int size;
    int width;
    int height;
} figures;

map<string, figure> Figure = {
    pair<string, figure>{"triangle", (figure){triangle, 14, 7, 5}},
    pair<string, figure>{"square", (figure){square,   16, 5, 5}},
    pair<string, figure>{"circle", (figure){circle,   12, 5, 5}}
};

void drawFigure(bmp *b, figure fig, int x, int y){
    for(int s = 0; s<fig.size; s++)
        if(x+fig.coords[s].x<7 && y+fig.coords[s].y<7)
            set(b,x+fig.coords[s].x,y+fig.coords[s].y,1);
}

void clearBMP(bmp* b){
    for(int i=0, j=0; j<7; j+=i==6, i=i==6 ? 0: i+1){
        set(b, i, j, 0);
    }
}


struct bmp* read(char* filename){
    FILE* f = fopen("img.bmp", "r");
    struct bmp b;
    char* buf = (char*) malloc(sizeof(struct bmp));
    fread(buf, 1, sizeof(struct bmp), f);
    return (struct bmp*) buf;
}

void save(char* filename, bmp *b)
{
    FILE * fp;
    char *c;
    int size = sizeof(bmp);
  
    fp = fopen(filename, "wb");
    c = (char *)b;
    for (int i = 0; i < size; i++)
        putc(*c++, fp);
    fclose(fp);
}

void makeImages(){
    struct bmp *b = read((char*) "img.bmp");
    clearBMP(b);
    drawFigure(b, Figure["square"], 0, 0);
    save((char*)"img_created.bmp", b);
}

int main(){
    makeImages();
}