#pragma once
#ifndef NUMATYPE_HPP
#define NUMATYPE_HPP


#include <utility>
#include <type_traits>
#include <numa.h>
#include <numaif.h> //set_mempolicy
#include <unistd.h> //getpagesize
#include <numa.h>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <cassert>
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

    template <typename U>
    struct rebind {
        using other = NumaAllocator<U, NodeID>;
    };

    NumaAllocator() noexcept {}         //error handling. returns true if no exception handling is done
    template <typename U>
    NumaAllocator(const NumaAllocator<U, NodeID>&) noexcept {}

    pointer allocate(size_type n) {
        //std::cout << "Allocated on numa node: " << NodeID <<std::endl;
        void* p = numa_alloc_onnode(n * sizeof(T), NodeID);
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type n) noexcept {
        numa_free(p, n * sizeof(T));
    }

    template <typename U, typename... Args>                                     //What is this??
    void construct(U* p, Args&&... args) {
        ::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);       //some form of wrapping for the type-
    }

};

template <typename T1, int NodeID1, typename T2, int NodeID2>
bool operator==(const NumaAllocator<T1, NodeID1>&, const NumaAllocator<T2, NodeID2>&) noexcept {
    return NodeID1 == NodeID2;
}

template <typename T1, int NodeID1, typename T2, int NodeID2>
bool operator!=(const NumaAllocator<T1, NodeID1>&, const NumaAllocator<T2, NodeID2>&) noexcept {
    return NodeID1 != NodeID2;
}


inline int get_numa_node_id(void* ptr) {                   //what is going on here??
    int status[1];
    int ret_code;
    status[0]=-1;
    ret_code = move_pages(0 /*self memory */, 1, &ptr,
        NULL, status, 0);
    if (ret_code != 0) {
        throw std::runtime_error("move_pages failed");
    }
    return status[0];
}

template<typename T, int NodeID, template<typename,int> class Alloc = NumaAllocator , typename E=void > 
class numa; // declaration of full numa type



// primitive numa
template<typename T, int NodeID, template <typename, int> class Alloc>
class numa<T,NodeID, Alloc, typename std::enable_if<(std::is_fundamental<T>::value|| std::is_pointer<T>::value)>::type>{
public:
	T contents;
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
		store((T)0);
	}
    //constructor with multiple arguments
    // template<typename... Args>
    // numa(Args... args){
    //     store(T(args...));
    // }
	// inline operator T() {return load();}
    inline operator T&(){return contents;}
    
	inline T operator-> (){
        // std::cout<<"operator -> called"<<std::endl;
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

    //overload = operator
    numa& operator=(const T& data){
        store(data);
        return *this;
    }

};


//This is only needed pre compiling and it doesn't do anything after


template<typename T, int NodeID, template <typename, int> class Alloc>
class numa<T,NodeID, Alloc, typename std::enable_if<!(std::is_fundamental<T>::value || std::is_pointer<T>::value)>::type>: public T{
public:
    numa(){
        std::cout<<"numa constructor called"<<std::endl;
        assert(false && "This constructor should never get called");
    }    
};

#endif