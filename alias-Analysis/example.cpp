#include<iostream>

using namespace std;

int main(){
    int* a = new int(5);
    int b = 10;
    a = &b;
    int* c = a;
    return 0;
}
