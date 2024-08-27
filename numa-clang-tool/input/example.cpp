#include <iostream>
#include "../numaLib/numatype.hpp"
// template<typename T, int N>
// class numa{

// };


class MyVector{  
    public:
    int* data;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val);
    int getData(int idx);
    void setData(int idx, int value);
};


MyVector::MyVector(int sz, int val = 0){
    int assignment = val;
    data = new int[sz];
}


int MyVector::getData(int idx){
    return data[idx];
}

void MyVector::setData(int idx, int value){
    data[idx]=value;
}

template<>
class numa<MyVector,1>{
public:
    // numa<int,1>* data;
    numa<numa<int,1>,1> *data;

    numa(int sz, int val = 0){
    int assignment = val;
    data = new numa<int,1>[sz];
    // data = new int[sz];
}


int getData(int idx){
    return data[idx];
}

void setData(int idx, int value){
    data[idx]=value;
}

};


int main(){
    numa<MyVector,1> myvec;
    int value;
    myvec.data = value;
}