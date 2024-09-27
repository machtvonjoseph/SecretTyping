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
    int b;
    int c;
    void someOtherFunction();
    SomeOtherClass() : a(0), b(1), c(2)
	{}
    SomeOtherClass(int a, int b, int c) : a(a), b(b), c(c){}
};

void SomeOtherClass::someOtherFunction(){
    return;
}

// template<>
// class numa<SomeOtherClass,3>{
//     numa<int,3> a;
// };
#endif