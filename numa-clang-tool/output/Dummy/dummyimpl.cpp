#include "dummyheader.h"
#include "anotherdummyheader.h"
//#include "numatype.hpp"

class SomeClass{
    int a;
    MyVector mv;
    public:
        SomeClass(){};
        virtual void someFunction(){
            return;
        }
};

template<>
class numa<SomeClass,3>{
public:
numa():numa(){}
private:
numa<int,3> a;
numa<MyVector,3> mv;
};
int main(){
    numa<MyVector, 3>* v1 = new numa<MyVector, 3>();
    numa<SomeClass, 3>* v2 = new numa<SomeClass, 3>();
    int* dummyint = new int[10];
    // numa<SomeOtherClass, 3>* v3 = new numa<SomeOtherClass, 3>();
    numa<int,3>* v4= new numa<int,3>();
    //MyVector* v = new MyVector(10);
    dummyFunction();
    anotherdummyFunction();
    return 0;
}