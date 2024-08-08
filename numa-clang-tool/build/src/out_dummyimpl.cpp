#include "dummyheader.h"
#include "anotherdummyheader.h"
#include "numatype.hpp"
// template <typename T, int N>
// class numa{

// };
class SomeClass;
template<typename T, int NodeID>
class numa<T , NodeID, NumaAllocator, typename std::enable_if<std::is_same<T, SomeClass>::value>::type>{
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
static void* operator new(std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }
static void* operator new[](std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }
static void operator delete(void* ptr){
        std::cout<<"delete operator called"<<std::endl;
        allocator_type alloc;
        alloc.deallocate(static_cast<T*>(ptr), 1);
    }
static void operator delete[](void* ptr){
        allocator_type alloc;
        alloc.deallocate(static_cast<T*>(ptr), 1);
    }
};
class SomeClass{
    public:
    SomeClass(){};
    int a;
};

int main(){
    numa<MyVector, 3>* v2 = new numa<MyVector, 3>();
    numa<SomeClass, 3>* v3 = new numa<SomeClass, 3>();
    //numa<SomeOtherClass, 3>* v4 = new numa<SomeOtherClass, 3>();
    //MyVector* v = new MyVector(10);
    dummyFunction();
    anotherdummyFunction();
    return 0;
}