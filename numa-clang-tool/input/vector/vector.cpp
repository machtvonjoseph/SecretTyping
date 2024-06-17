#include "vector.h"
#include "someOtherClass.h"
#include "numatype.hpp"
#include <iostream>


using namespace std;

class SomeClass{
private:
    int a;
public:
    SomeClass(){
        a = 0;
    }
};

int main(){
    numa<MyVector*, 3> v2 = new numa<MyVector, 3>(10,3);
    numa<SomeClass*, 3> v3 = new numa<SomeClass, 3>();
    v2->setData(0, 5);
    int val = v2->getData(0);
    cout<<val<<endl;
    //numa<SomeOtherClass, 3>* v4 = new numa<SomeOtherClass, 3>();
    dummyFunction();
    anotherdummyFunction();
    return 0;
}