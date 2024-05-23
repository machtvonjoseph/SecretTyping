template<class T, int N>
class numa{

};

class OtherClass{
    public:
    int a;
    OtherClass(){
        a=0;
    }
    void my_func(int x, int y){
        a=y;
    }

};

class MyClass{
    public:
        MyClass();
        MyClass(int p){
            x = p;
        }
        int x;
        int y;
        OtherClass z;
};


int main()
{
    numa <MyClass, 7>* myobj = new numa<MyClass, 7>();
    MyClass m;
}


//T = {int:x, int:y, int:a, OtherClass:z, MyClass:m}
//K = {numa<MyClass, 7>:myobj}
//K' = { }

// template<class T, int N>
// class numa{

// };

// class OtherClass{
//     int a;
// };

// template<>
// class numa<OtherClass, 7 >{
//     numa<int, 7> a;
// };

// class MyClass;

// template<>
// class numa<MyClass, 7 >{
//     numa<int, 7> x;
//     numa<int, 7> y;
//     numa<OtherClass, 7> z;
// public:
// };

// class MyClass{
//     int x;
//     int y;
//     OtherClass z;
// };

// int main()
// {
//     numa <MyClass, 7> myobj;
//     MyClass m;
// }

//T2 ={MyClass:m, int:x, int:y, int:a, OtherClass:z}
//K2 = {numa<MyClass, 7>:myobj}
//K'2 = {numa<int, 7>:x, numa<int, 7>:y, numa<OtherClass, 7>:z, numa<int, 7>:a}
