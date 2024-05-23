//#include "numatype.hpp"

using namespace std;

template<class T, int N>
class numa{
    public:
    int get(){return 0;}
    void put(int val){}
};

class singleint{
    public:
    int* x;
    singleint(){x = new int(0);}

    int get(){return *x;}
    void put(int val){*x = val;}
};

int main(){
    numa<singleint,3>* v = new numa<singleint,3>();
    v->put(10);
    int c = v->get();
}