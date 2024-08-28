#ifndef DUMMYHEADER_H
#define DUMMYHEADER_H

#include "anotherdummyheader.h"
//#include "numatype.hpp"
void dummyFunction(){
    return;
}
template <typename T, int N>
class numa{

};
class MyVector{  
    public:
    int* data;
    SomeOtherClass* soc;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val);
    virtual int getData(int idx);
    virtual void setData(int idx, int value);
};

template<>
class numa<MyVector,3>{
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


#endif