using namespace std;
class bmp{
    public:

        char header[54];
        struct point{
        char p[3];
        } data[7][8];

        bmp(){}

        bmp(char filename[]){
            FILE* f = fopen(filename, "r");
            if(!f) {
                cout<<"ERROR "<<filename<<endl;
                exit(0);
            }
            fread(header, 1, sizeof(header), f);

            for(int i=0; i<7; i++)
                for(int j=0; j<8; j++){
                    point p;
                    fread(&p, sizeof(point), 1, f);
                    data[i][j]= p; 
                }
            fclose(f);
        }

        void clear(){
            for(int i=0; i<7; i++)
                for(int j=0; j<8; j++)
                    data[i][j]=(point){0,0,0};
        }

        int size(){
            return sizeof(header) + sizeof(data);
        }

        void set(int i, int j, int value){
            data[6-j][i] = value==0 
                    ? (point){-1,-1,-1} 
                    : (point){ 0, 0, 0};
        }

        int get(int i, int j){
            return data[6-i][j].p[0] == 0;
        }

        int get(int i){
            return get(i/7, i%7);
        }

};