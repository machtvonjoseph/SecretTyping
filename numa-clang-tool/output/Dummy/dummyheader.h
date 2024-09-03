#ifndef DUMMYHEADER_H
#define DUMMYHEADER_H

#include "anotherdummyheader.h"
//#include "numatype.hpp"
void dummyFunction(){
    return;
}

class MyVector{  
    public:
    int* data;
    SomeOtherClass* soc;
    int valueData;
    SomeOtherClass valuesoc;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val);
    virtual int getData(int idx);
    virtual void setData(int idx, int value);
};

template<>
class numa<MyVector,3>{
numa<int,3>* data;
numa<SomeOtherClass,3>* soc;
numa<int,3> valueData;
numa<SomeOtherClass,3> valuesoc;
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

// template<>
// class numa<MyVector,3>{
// numa<int,3>* data;
// numa<SomeOtherClass,3>* soc;

// };
#endif