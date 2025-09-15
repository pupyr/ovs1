#include <stdio.h>
#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <random>
#include "bmp.cpp"
#include "figure.cpp"
using namespace std;

#define layers_num 1
#define layers_sizes {3}
#define default_weight 0
double a = 5;
#define epoch 10
#define threshold 0.85

struct point{
    vector<double> weights;
    double err;
    double out;
};

int sizes[layers_num] = layers_sizes;
vector<point> nn[layers_num];

void initNN(){
    for(int i=0; i<layers_num; i++)
        for(size_t j=0; j<sizes[i]; j++){
            vector<double> weights(i==0 ? 7*7 : sizes[i-1], default_weight);
            nn[i].push_back((point){.weights=weights});
        }
}

void set_out(bmp b, int layer_id, int elem_id){
    point p = nn[layer_id][elem_id];
    double sum = 0;
    for(int i=0; i<p.weights.size(); i++){
        sum +=  (layer_id != 0 
                    ? nn[layer_id-1][i].out
                    : b.get(i))
                * p.weights[i];
    }
    nn[layer_id][elem_id].out = 1.0/(1.0 + exp(-sum));
    // cout<<1.0/(1.0 + exp(-sum))<<endl;
}

void answer(bmp b){
    for(int i=0; i<layers_num; i++)
        for(int j=0; j<sizes[i]; j++)
            set_out(b,i,j);
}

double middle_err(int lay, int i){
    double last_out = nn[lay][i].out;
    double sum = 0;
    for(int j=0; j<sizes[lay+1]; j++)
        sum+=nn[lay+1][j].err*nn[lay+1][j].weights[i];
    return last_out*(1.0-last_out)*sum;
}

double last_err(int right_answer, int lay, int i){
    double last_out = nn[lay][i].out;
    return last_out*(1.0-last_out)*(((double)right_answer==i) - last_out);
}

void correct_weights(bmp b, int right_answer){
    for(int l=layers_num-1; l>=0; l--){
        for(int i=0; i<sizes[l]; i++){
            double err = l==(layers_num-1)
                                ? last_err(right_answer,l,i)
                                : middle_err(l,i);
            nn[l][i].err = err;
            for(int j=0; j<nn[l][i].weights.size(); j++){
                nn[l][i].weights[j] += a*err* ( l!=0
                                                        ? nn[l-1][j].out
                                                        : b.get(j));
                }
        }
    }
}

void learning(){
    vector<string> v;
    for(pair<string, figure> p: Figure)
        for(int i=0; i<=/*7-p.second.width*/0; i++)
            for(int j=0; j<=/*7-p.second.height*/2; j++){
                char s[30];
                string str = p.first;
                sprintf(s, "imgs/%s/%d_%d.bmp",str.c_str(),i,j);
                v.push_back(s);
            }
    auto rng = default_random_engine {};
    shuffle(begin(v), end(v), rng);
    int num = 0;
    for(pair<string, figure> p: Figure)
        for(int i=0; i<=7-p.second.width; i++)
            for(int j=0; j<=7-p.second.height; j++){
                if(i!=0 || j>2) continue;
                char *str = (char*) v[num++].c_str();
                bmp b = bmp(str);
                answer(b);
                correct_weights(b, str[5] == 't' ? 0
                                                 : (str[5] == 's' ? 1
                                                                  : 2));
            }
}

int moreThreshold(int id){
    return nn[layers_num-1][id].out > threshold;
}

void test(char* str){
    cout<<"test "<<str<<" started:"<<endl;
    bmp test = bmp(str);
    answer(test);
    if(moreThreshold(0)) cout<<"Triangle"<<endl;
    if(moreThreshold(1)) cout<<"Square"<<endl;
    if(moreThreshold(2)) cout<<"Circle"<<endl;
    // cout<< nn[layers_num-1][0].out<<" "<<nn[layers_num-1][1].out<<" "<<nn[layers_num-1][2].out<<endl;
    cout<<endl;
}

int main(){
    initNN();
    for(int i=0; i<epoch; i++){
        learning();
        a*=0.8;
    }

    test((char*)"imgs/triangle.bmp");
    test((char*)"imgs/triangle_break.bmp");
    test((char*)"imgs/circle.bmp");
    test((char*)"imgs/circle_break.bmp");
    test((char*)"imgs/square.bmp");
    test((char*)"imgs/square_break.bmp");
}