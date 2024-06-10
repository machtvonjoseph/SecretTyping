#include<iostream>


template<typename T>
class numa{
    public:
        numa<T> operator=(T data){
            return *this;
        }
};

class MyClass{

    public:
    MyClass operator=(int val){
        return *this;
    }
};

int main(){
    MyClass obj;
    obj = 10;
    numa<int*> a;
    int* b;
    a = b;
    return 0;
}



