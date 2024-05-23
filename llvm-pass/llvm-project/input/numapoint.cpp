#include "numatype.hpp"

using namespace std;

// template<class T, int N>
// class numa{
//     public:
//     int get(){return 0;}
//     void put(int val){}
// };

class singleint;
template<typename T, int NodeID>
class numa<T , NodeID, NumaAllocator, typename std::enable_if<std::is_same<T, singleint>::value>::type>{
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
       numa<int *, 3> x;
public:
numa();
int get(){return *x;}
void put(int val){*x = val;}
};
class singleint{
    public:
    int* x;
    singleint(){x = new int(0);}

    int get(){return *x;}
    void put(int val){*x = val;}
};

int main(){
    numa<singleint,3>* v = new numa<singleint,3>();
    v->put(10);
    int c = v->get();
}