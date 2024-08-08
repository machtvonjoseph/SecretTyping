#ifndef DUMMYHEADER_H
#define DUMMYHEADER_H

void dummyFunction(){
    return;
}
template <typename T, int N>
class numa{

};


class MyVector{  
    public:
    int* data;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        int assignment = val;
        data = new int[sz];
    }
    int getData(int idx){
        return data[idx];
    }
    void setData(int idx, int value){
        data[idx] = value;
    }
};


template<>
class numa<MyVector, 3>:public MyVector{
    public:
    MyVector* data;
    numa(){
        data = new MyVector[3];
    }
    ~numa(){
        delete[] data;
    }                                               
};


#endif