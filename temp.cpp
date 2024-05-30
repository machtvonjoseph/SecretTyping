
#include <utility>
#include <type_traits>
#include <numa.h>
#include <numaif.h> //set_mempolicy
#include <unistd.h> //getpagesize
#include <numa.h>
#include <memory>
#include <stdexcept>
#include <iostream>

template <typename T, int NodeID>
class NumaAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // template <typename U>
    // struct rebind {
    //     using other = NumaAllocator<U, NodeID>;
    // };

    // NumaAllocator() noexcept {}         //error handling. returns true if no exception handling is done
    // template <typename U>
    // NumaAllocator(const NumaAllocator<U, NodeID>&) noexcept {}

    pointer allocate(size_type n) {
        //std::cout << "Allocated on numa node: " << NodeID <<std::endl;
        void* p = numa_alloc_onnode(n * sizeof(T), NodeID);
        //void* p = malloc(n * sizeof(T));
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return static_cast<pointer>(p);
    }

    // void deallocate(pointer p, size_type n) noexcept {
    //     numa_free(p, n * sizeof(T));
    // }

    template <typename U, typename... Args>                                     //What is this??
    T* construct(U* p, Args&&... args) {
        //::new (p) U(std::forward<Args>(args)...);       //some form of wrapping for the type-
        T* obj = new (p) T(std::forward<Args>(args)...);
        return obj;
    }
    

//     template <typename T,  typename... Args>
//     T* numa_construct_onnode(int node, Args&&... args) {
//     // Allocate memory on the specified NUMA node
//     void* raw_memory = numa_alloc_onnode(sizeof(T), node);
//     if (!raw_memory) {
//         throw std::bad_alloc();
//     }

//     try {
//         // Construct the object using placement new
//         T* obj = new (raw_memory) T(std::forward<Args>(args)...);
//         return obj;
//     } catch (...) {
//         // If construction fails, free the allocated memory
//         numa_free(raw_memory, sizeof(T));
//         throw;
//     }
// }

};

template<typename T, int NodeID, template<typename,int> class Alloc = NumaAllocator , typename E=void >
class numa; // declaration of full numa type

template<typename T, int NodeID, template <typename, int> class Alloc>
class numa<T,NodeID, Alloc, typename std::enable_if<!(std::is_fundamental<T>::value || std::is_pointer<T>::value)>::type>: public T{
public:
    using allocator_type = Alloc<T, NodeID>;
    static void* operator new(std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }

    static void* operator new[](std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }
};

class MyClass{
    public:
    MyClass(){};
    int a;

    void set_a (int val){
        a = val;
    }
    int get_a(){
        return a;
    }
};
int main() {
    numa<MyClass, 0>* n2 = new numa<MyClass, 0>();
    n2->set_a(10);
    std::cout << n2->get_a() << std::endl;
    return 0;
}