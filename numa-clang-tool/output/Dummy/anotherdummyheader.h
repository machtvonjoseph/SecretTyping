#include "numatype.hpp"
#ifndef ANOTHERDUMMYHEADER_H
#define ANOTHERDUMMYHEADER_H
template <typename T, int N>
class numa{

};
void anotherdummyFunction(){
    return;
}
class SomeOtherClass{
    public:
    //SomeOtherClass(){};
    int a;

    virtual void someOtherFunction();
    SomeOtherClass() : a(0)
	{}
};

template<>
class numa<SomeOtherClass,3>{
public:
numa<int,3> a;
numa():numa(0)
	{}
private:
};

void SomeOtherClass::someOtherFunction(){
    return;
}

// template<>
// class numa<SomeOtherClass,3>{
//     numa<int,3> a;
// };
#endif