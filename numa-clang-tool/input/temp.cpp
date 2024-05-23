#include<iostream>

using namespace std;

template<class T, int N>
class numa{
    public:
    void my_func(){
        
    }
    void ur_func(){
        
    }
};

class MyVector{  
    public:
    int* my_data;
    //SomeClass* x;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        int* container = new int[sz];
        my_data = container;
    }
    int getmy_data(int idx){
        //x = new SomeClass();
        return my_data[idx];
    }
    void setmydata(int idx, int value){
        //my_data[idx] = value;
        int *val;
        my_data= val;
    }
};

int main(){          
    numa<MyVector,3>* v = new numa<MyVector,3>; 
}