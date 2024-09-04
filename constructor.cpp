#include <iostream>

using namespace std;

class MyClass {
    int x;
    public:
        MyClass():MyClass(0){};
        MyClass(int x){
            this->x = x;
        }
};

int main() {
    MyClass obj;
    return 0;
}