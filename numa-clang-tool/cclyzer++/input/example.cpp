class SomeClass{
    int a;
};


class MyVector{  
    public:
    int* some_int;
    SomeClass* some_class; //numa resident pointer regular target
    SomeClass obj;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        some_int = new int[sz];
        some_class = new SomeClass[sz];
        some_class = &obj; 
    }


};





int main(){    
    // numa<int, 3>* v2 = new numa<int, 3>();        
    // numa<MyVector,3>* v = new numa<MyVector,3>(10,3);   
    // MyVector* v3 = new MyVector(10,3);
    MyVector v3;

}