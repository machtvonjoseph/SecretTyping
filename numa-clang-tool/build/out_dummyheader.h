#include "dummyheader.h"
#include "anotherdummyheader.h"
//#include "numatype.hpp"
template <typename T, int N>
class numa{

};
class SomeClass{
    public:
    SomeClass(){};
    int a;
};

int main(){
    numa<MyVector, 3>* v2 = new numa<MyVector, 3>();
    numa<SomeClass, 3>* v3 = new numa<SomeClass, 3>();
    //numa<SomeOtherClass, 3>* v4 = new numa<SomeOtherClass, 3>();
    //MyVector* v = new MyVector(10);
    dummyFunction();
    anotherdummyFunction();
    return 0;
}