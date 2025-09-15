#include <map>
using namespace std;

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