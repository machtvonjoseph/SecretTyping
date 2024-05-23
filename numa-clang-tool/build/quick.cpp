#include <iostream>

template<class T, int N>
class numa{

};

class OtherClass;
template<>
class numa<MyClass, 1>{
    numa<int, 1> x;
    numa<OtherClass,1>* ocp;
};

class MyClass {
    public:
    int x;
    OtherClass* ocp;
    MyClass();
};


class OtherClass{
    int y;
    public:
    OtherClass();
};

int main(){
    MyClass* mc = new MyClass();
    numa<MyClass,1>* numa_mc = new numa<MyClass,1>();
    OtherClass* oc = new OtherClass();
    mc->ocp = oc;
}
