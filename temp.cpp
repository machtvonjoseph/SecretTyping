
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

};

template<typename T, int NodeID, template<typename,int> class Alloc = NumaAllocator , typename E=void >
class numa; // declaration of full numa type

// primitive numa
template<typename T, int NodeID, template <typename, int> class Alloc>
class numa<T,NodeID, Alloc, typename std::enable_if<(std::is_fundamental<T>::value || std::is_pointer<T>::value)>::type>{
public:
	T contents;

public:
	using allocator_type = Alloc<T,NodeID>;
	
	inline T load()
	__attribute__((always_inline)){
		return contents;
	}

	inline void store(T data)
	__attribute__((always_inline)){
		contents = data;
	}
	numa(T data){
		store(data);
	}
	numa(){
        std::cout<<"default constructor for int"<<std::endl;
		store((T)0);
	}
    //constructor with multiple arguments
    template<typename... Args>
    numa(Args... args){
        store(T(args...));
    }
	// inline operator T() {return load();}
    inline operator T&(){return contents;}
    
	inline T operator-> (){
        std::cout<<"->operator being used"<<std::endl;
		static_assert(std::is_pointer<T>::value,"-> operator is only valid for pointer types");
		return load();
	}

    static void* operator new(std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }

    static void* operator new[](std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }

    T& operator=(numa<T,NodeID> data){
        store(data);
        return *this;
    } 

    // static T operator=(T data){
    //     return data;
    // }

};

template<typename T, int NodeID, template <typename, int> class Alloc>
class numa<T,NodeID, Alloc, typename std::enable_if<!(std::is_fundamental<T>::value || std::is_pointer<T>::value)>::type>: public T{
public:
    using allocator_type = Alloc<T, NodeID>;
    static void* operator new(std::size_t sz){
        std::cout<<"new operator for complex type"<<std::endl;
		allocator_type alloc;
        return alloc.allocate(sz);
    }

    static void* operator new[](std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }
        
	inline T operator-> (){
		static_assert(std::is_pointer<T>::value,"-> operator is only valid for pointer types");
		return *this;
	}
};

class MyClass{
    public:
    MyClass(){};
    int a;

    void set_a (int val){
        a = val;
    }
    int* get_a(){
        return &a;
    }

    MyClass get_next(){
        return MyClass();
    }
};
int main() {

    numa<int*,0> n1 = new numa<int,0>();
    int* n3 = new int();
    n1= n3;
    n3=n1;

    numa<MyClass*, 0> n2 = new numa<MyClass, 0>();
    n2->set_a(10);
    n1=n2->get_a();
    MyClass* n4 = new MyClass();
    n2 = n4;
    n4 = n2;
    std::cout<<*n1<<std::endl;
    return 0;
}