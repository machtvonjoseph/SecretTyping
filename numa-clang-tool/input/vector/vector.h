#ifndef DUMMYHEADER_H
#define DUMMYHEADER_H

#include "someOtherClass.h"
void dummyFunction(){
    return;
}

class MyVector{  
    public:
    int* data;
    SomeOtherClass* soc;
public:
    MyVector(int sz, int val);
    int getData(int idx);
    void setData(int idx, int value);
};

MyVector::MyVector(int sz, int val){
    data = new int[sz];
}
int MyVector::getData(int idx){
    return data[idx];
}
void MyVector::setData(int idx, int value){
    data[idx] = value;
}

#endif