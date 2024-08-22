#pragma once

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


int get_numa_node_id(void* ptr) {                   //what is going on here??
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
		store((T)0);
	}
    //constructor with multiple arguments
    template<typename... Args>
    numa(Args... args){
        store(T(args...));
    }
	// inline operator T() {return load();}
    inline operator T&(){return contents;}
    
	// inline T operator-> (){
	// 	static_assert(std::is_pointer<T>::value,"-> operator is only valid for pointer types");
	// 	return load();
	// }
	
    static void* operator new(std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }

    static void* operator new[](std::size_t sz){
		allocator_type alloc;
        return alloc.allocate(sz);
    }

};


// // complex numa
// template<typename T, int NodeID, template <typename, int> class Alloc>
// class numa<T,NodeID, Alloc, typename std::enable_if<!(std::is_fundamental<T>::value || std::is_pointer<T>::value)>::type>: public T{
// public:
//     using allocator_type = Alloc<T, NodeID>;
//     //using pointer_alloc_type =Alloc<T*, NodeID>;
//     // numa() {
//     //     allocator_type alloc;
//     //     T* p = alloc.allocate(1);
//     //     alloc.construct(p, T());                //If possible, construct an object of type T in allocated unititialized storage pointed to by p
//     //                                             //but in our case we are only handling objects with no member functions(??)
//     //     this->swap(*p); // point to the allocated memory
//     // }
//     // numa(size_t size){                          //This is to allocate a custom size. Like in a vector
//     //     allocator_type alloc;
//     //     T* p = alloc.allocate(size);
//     //     alloc.construct(p, T());                
//     //     this->swap(*p); // point to the allocated memory
//     // }
//     // explicit numa(const allocator_type& alloc) noexcept : T(alloc) {}
//     // explicit numa(const T& t, const allocator_type& alloc = allocator_type()) : T(t, alloc) {}
//     // explicit numa(T&& t, const allocator_type& alloc = allocator_type()) : T(std::move(t), alloc) {}    
    
//     numa(T t) : T(t) {}
//     // inline operator T() {return *this;}
//     inline operator T&(){return *this;}
// 	// inline T operator-> (){
// 	// 	static_assert(std::is_pointer<T>::value,"-> operator is only valid for pointer types");
// 	// 	return this;
// 	// }

//     static void* operator new(std::size_t count)
//     {
//         allocator_type alloc;
//         return alloc.allocate(count);
//     }
 
//     static void* operator new[](std::size_t count)
//     {
//         //todo: disable placement new for numa.
//         allocator_type alloc;
//         return alloc.allocate(count);
//     }
	
//     // assignment operators, use NumaAllocator
//     // numa& operator=(const T& t) {
//     //     allocator_type alloc;
//     //     T* p = alloc.allocate(1);       //1 * sizeof(T) bytes allocated
//     //     alloc.construct(p, t);      //construct then swap
//     //     this->swap(*p);
//     //     return *this;
//     // }

//     // numa& operator[](std::size_t index) {
//     //     static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
//     //     return this[index];
//     // }

//     // // Const version of the [] operator
//     // const T& operator[](std::size_t index) const {
//     //     static_assert(std::is_pointer<T>::value,"[] operator is only valid for pointer types");
//     //     return this[index];
//     // }

//     int node_id = NodeID;
//     constexpr operator int() const { return NodeID; }
// };

