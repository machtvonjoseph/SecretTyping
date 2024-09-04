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
    SomeOtherClass(){};
    int a;

    virtual void someOtherFunction();
};

template<>
class numa<SomeOtherClass,3>{
public:
numa<int,3> a;
numa();
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