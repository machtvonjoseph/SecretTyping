//TODO: This has erronous result. As it tries to generate a type for numa<Bar,7>:l19 as numa<numa<bar,7>,3>, 
//it will look for members of a class named numa<Bar,7> which does not exist.
template<class T, int N>
class numa{

};

class Bar;
class OtherClass{
    int a;
};

class MyClass{
    int x;
    int y;
    OtherClass z;
};

class Foo{
    numa<Bar, 5> f;
};


int main()
{
    numa <MyClass, 7> myobj;
    MyClass m;
    numa<Foo, 3> myfoo;
    Foo foo;
}

//T = { MyClass:m, int:x, int:y, OtherClass:z, int:a, Foo:f}
//K = {numa<MyClass, 7>:myobj, numa<Foo, 3>:f , numa<Bar, 7>:f}
//K' = { }

template<class T, int N>
class numa{

};

class Bar;
class OtherClass{
    int a;
};

template<>
class numa<OtherClass, 7 >{
    numa<int, 7> a;
};

class MyClass;
template<>
class numa<MyClass, 7 >{
    numa<int, 7> x;
    numa<int, 7> y;
    numa<OtherClass, 7> z;
};

class MyClass{
    int x;
    int y;
    OtherClass z;
};


template<>
class numa<numa<Bar, 7>, 3>{
                                     
};

class Foo;
template<>
class numa<Foo, 3>{                //SHOULD be numa<Foo, (3 or 7)>{ ...
    //Here is a problem.
    numa<numa<Bar, 7>, 3> f;    //NOT Allowed : SHOULD be numa<Bar, (3 or 7)>;
                           
};

class Foo{
    numa<Bar, 7> f;
};

int main()
{
    numa <MyClass, 7> myobj;
    MyClass m;
    numa<Foo, 3> myfoo;
    Foo foo;
}

//T2 ={int:x, int:y, int:a, OtherClass:z, MyClass:m}
//K2 = {numa<MyClass, 7>:myobj}
//K'2 = {numa<int, 7>:x, numa<int, 7>:y, numa<OtherClass, 7>:z, numa<int, 7>:a}



//T3 = {int:x, int:y, int:a, OtherClass:z, MyClass:m, Foo:foo}
//K3 = {numa<MyClass, 7>:myobj, numa<Foo, 3>:myfoo, numa<Bar, 7>:f}
//K'3 = {numa<int>:x, numa<int>:y, numa<OtherClass>:z, numa<int>:a, numa<numa<Bar, 7>,3>:f, empty }

