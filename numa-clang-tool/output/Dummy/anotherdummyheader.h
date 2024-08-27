#ifndef ANOTHERDUMMYHEADER_H
#define ANOTHERDUMMYHEADER_H

void anotherdummyFunction(){
    return;
}
class SomeOtherClass{
    public:
    SomeOtherClass();
    int a;

    virtual void someOtherFunction();
};

template<>
class numa<SomeOtherClass,3>{
};

void SomeOtherClass::someOtherFunction(){
    return;
}


#endif