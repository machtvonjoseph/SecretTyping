//#include<iostream>
using namespace std;
class my_class{
// public:
//     void my_func(){
//         int i;
//         char C[2];
//         char A[10];
//         /* ... */
//         for (i = 0; i != 10; ++i) {
//         C[0] = A[i];          /* One byte store */
//         C[1] = A[9-i];        /* One byte store */
//         }
//     }
};

int main(){
    int x=3;
    // cout<<x<<endl;
    int* y = &x;
    int *w =&x;
    y = w;
    *w = 4;
    // cout<<x<<endl; // prints 4
    *y = 5;
    // cout<<x<<endl; // prints 5

    // my_class mc;
    // mc.my_func();
}

