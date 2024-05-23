#ifndef DUMMYHEADER_H
#define DUMMYHEADER_H

void dummyFunction(){
    return;
}



class MyVector{  
    public:
    int* data;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        data = new int[sz];
    }
    int getData(int idx){
        return data[idx];
    }
    void setData(int idx, int value){
        data[idx] = value;
    }
};

#endif