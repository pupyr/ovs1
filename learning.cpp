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

#ifdef lay1
#define layers_num 1
#define layers_sizes {3}
#define default_weight 0
double a = 0.1;
#define epoch 1000
#define a_0 1
#define threshold 0.7
#endif

#ifdef lay2
#define layers_num 2
#define layers_sizes {1,3}
#define default_weight 0.5
double a = 5;
#define epoch 20
#define a_0 1
#define threshold 0.8
#endif

struct point{
    vector<double> weights;
    vector<double> weights_tmps;
    double err;
    double out;
};

int sizes[layers_num] = layers_sizes;
vector<point> nn[layers_num];

void initNN(){
    for(int i=0; i<layers_num; i++)
        for(size_t j=0; j<sizes[i]; j++){
            vector<double> weights(i==0 ? 7*7 : sizes[i-1], default_weight);
            vector<double> weights_tmps(i==0 ? 7*7 : sizes[i-1], default_weight);
            nn[i].push_back((point){.weights=weights, .weights_tmps=weights_tmps});
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
    nn[layer_id][elem_id].out = 1.0/(1.0 + exp(-sum))- 0.5 * (layer_id!=layers_num-1);
    // cout<<sum<<" "<<nn[layer_id][elem_id].out<<endl;
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
    // cout<<last_out*(1.0-last_out)*sum<<"____"<<endl;
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
                double answer = a*err* ( l!=0
                                            ? nn[l-1][j].out
                                            : b.get(j));
                // cout<<l<<" "<<i<<" "<<j<<" "<<err<<" "<<answer<<" "<<b.get(j)<<endl;
                nn[l][i].weights_tmps[j] = nn[l][i].weights[j] + answer;
                }
        }
    }
    for(int l=layers_num-1; l>=0; l--)
        for(int i=0; i<sizes[l]; i++)
            for(int j=0; j<nn[l][i].weights.size(); j++)
                nn[l][i].weights[j] = nn[l][i].weights_tmps[j];
}
auto rng = default_random_engine {};
void learning(){
    vector<string> v;
    for(pair<string, figure> p: Figure)
        for(int i=0; i<=/*7-p.second.width*/2; i++)
            for(int j=0; j<=/*7-p.second.height*/2; j++){
                char s[30];
                string str = p.first;
                sprintf(s, "imgs/%s/%d_%d.bmp",str.c_str(),i,j);
                v.push_back(s);
            }
    // shuffle(begin(v), end(v), rng);
    int num = 0;
    for(pair<string, figure> p: Figure)
        for(int i=0; i<=/*7-p.second.width*/2; i++)
            for(int j=0; j<=/*7-p.second.height*/2; j++){
                char *str = (char*) v[num++].c_str();
                // if(p.first=="square") str = "imgs/square.bmp";
                // else str = "imgs/triangle.bmp";
                // cout<<str<<endl;
                bmp b = bmp(str);
                for(int q=0;q<1;q++){
                    answer(b);
                    correct_weights(b, str[5] == 't' ? 0
                                                     : (str[5] == 's' ? 1
                                                                      : 2));
                }
            }
    // exit(0);
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
    cout<< nn[layers_num-1][0].out<<" "<<nn[layers_num-1][1].out<<" "<<nn[layers_num-1][2].out<<endl;
    cout<<endl;
}

int main(){
    initNN();
    for(int i=0; i<epoch; i++){
        learning();
        a*=a_0;
    }

    test((char*)"imgs/triangle.bmp");
    test((char*)"imgs/triangle_break.bmp");
    #ifdef lay1
    test((char*)"imgs/circle.bmp");
    test((char*)"imgs/circle_break.bmp");
    #endif
    test((char*)"imgs/square.bmp");
    test((char*)"imgs/square_break.bmp");
}