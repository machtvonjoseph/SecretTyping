#include "numatype.hpp"
#ifndef DUMMYHEADER_H
#define DUMMYHEADER_H

#include "anotherdummyheader.h"
//#include "numatype.hpp"
void dummyFunction(){
    return;
}

class MyVector{  
    virtual void setData(int idx, int value);
public:
    int* data;
    SomeOtherClass* soc;
    int valueData;
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val);
private:
    SomeOtherClass valuesoc;
    virtual int getData(int idx);
    
};

template<>
class numa<MyVector,3>{
public:
numa<int,3>* data;
numa<SomeOtherClass,3>* soc;
numa<int,3> valueData;
numa():numa(10,0){}
numa(int sz, int val){
    int assignment = val;
    data = new int[sz];
}
private:
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