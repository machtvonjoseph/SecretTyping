#include<iostream>

// #include"anothertemp.hpp"

using namespace std;


template<typename T>
class MyClass: public T{
    int b;
    public:
    MyClass(int b){}
    void print(){
        std::cout << " b: " << b << std::endl;
    }
};
class MyBaseClass{
    public:
    int a;
    
    MyBaseClass(int a):a(a){}
    void print(){
        std::cout << "a: " << a << std::endl;
    }
};



int main(){
    
    MyBaseClass* baseclassptr;
    MyClass<MyBaseClass>* myclassptr;
    myclassptr= baseclassptr;

}