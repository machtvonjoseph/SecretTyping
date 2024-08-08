//#include "numatype.hpp"
//#include <iostream>
// using namespace std;


template<class T, int N>
class numa{
    public:
    void my_func(){
        
    }
    void ur_func(){
        
    }
};

class SomeClass{
    public:
    SomeClass();
    int a;
};

class MyVector{  
    public:
    int* data;
    SomeClass* x;
    SomeClass y;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        data = new int[sz];
    }
    int getData(int idx){
        x = new SomeClass();
        return data[idx];
    }
    void setData(int idx, int value){
        data[idx] = value;
    }
};

int main(){    
    numa<int, 3>* v2 = new numa<int, 3>();        
    numa<MyVector,3>* v = new numa<MyVector,3>();   
}


