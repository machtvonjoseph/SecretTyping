#include"numatype.hpp"

using namespace std;


// template<class T, int N>
// class numa{
//     public:
//     void my_func(){
        
//     }
//     void ur_func(){
        
//     }
// };

class SomeClass{
    public:
    SomeClass();
    int a;
};

template< typename T, int NodeID>
class numa< T, NodeID , NumaAllocator, typename std::enable_if<std::is_same<T, SomeClass>::value>::type>{
using allocator_type = NumaAllocator<T,NodeID>;// Alloc<T, NodeID>;
using pointer_alloc_type =NumaAllocator<T*,NodeID>; //Alloc<T*, NodeID>;
public:
       
    numa(T t) : T(t) {}
    inline operator T() {return *this;}
    inline operator T&(){return *this;}
        inline T operator-> (){
                static_assert(std::is_pointer<T>::value,"-> operator is only valid for pointer types");
                return this;
        }

    static void* operator new(std::size_t count)
    {
        allocator_type alloc;
        return alloc.allocate(count);
    }
 
    static void* operator new[](std::size_t count)
    {
        //todo: disable placement new for numa.
        allocator_type alloc;
        return alloc.allocate(count);
    }

    numa& operator[](std::size_t index) {
        static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
        return this[index];
    }

    // Const version of the [] operator
    const T& operator[](std::size_t index) const {
        static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
        return this[index];
    }

    int node_id = NodeID;
    constexpr operator int() const { return NodeID; }
       numa<int, 3> a;
public:
numa();
void my_func(){
        
    }
void ur_func(){
        
    }
};
class MyVector;
template<typename T, int NodeID>
class numa<T , NodeID, NumaAllocator, typename std::enable_if<std::is_same<T, MyVector>::value>::type>{
using allocator_type = NumaAllocator<T,NodeID>;// Alloc<T, NodeID>;
using pointer_alloc_type =NumaAllocator<T*,NodeID>; //Alloc<T*, NodeID>;
public:
       
    numa(T t) : T(t) {}
    inline operator T() {return *this;}
    inline operator T&(){return *this;}
        inline T operator-> (){
                static_assert(std::is_pointer<T>::value,"-> operator is only valid for pointer types");
                return this;
        }

    static void* operator new(std::size_t count)
    {
        allocator_type alloc;
        return alloc.allocate(count);
    }
 
    static void* operator new[](std::size_t count)
    {
        //todo: disable placement new for numa.
        allocator_type alloc;
        return alloc.allocate(count);
    }

    numa& operator[](std::size_t index) {
        static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
        return this[index];
    }

    // Const version of the [] operator
    const T& operator[](std::size_t index) const {
        static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
        return this[index];
    }

    int node_id = NodeID;
    constexpr operator int() const { return NodeID; }
    numa<int *, 3> data;
   //numa<SomeClass *, 3> x;
public:
numa():numa(10,0){}
numa(int sz, int val){
        data = new int[sz];
    }
int getData(int idx){
        //x = new SomeClass();
        return data[idx];
    }
void setData(int idx, int value){
        data[idx] = value;
    }
void my_func(){
        
    }
void ur_func(){
        
    }
};
class MyVector{  
    public:
    int* data;
    //SomeClass* x;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        data = new int[sz];
    }
    int getData(int idx){
        //x = new SomeClass();
        return data[idx];
    }
    void setData(int idx, int value){
        data[idx] = value;
    }
};

int main(){    
    // numa<int, 3>* v2 = new numa<int, 3>();        
    numa<MyVector,3>* v = new numa<MyVector,3>(); 
    v->getData(0);  
    v->setData(0, 10);
    //v->data = new numa<int,3>();                      //Idk what the problem is here
//must support v= new MyVector(10)        AST handling is different
}