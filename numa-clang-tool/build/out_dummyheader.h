#ifndef DUMMYHEADER_H
#define DUMMYHEADER_H

void dummyFunction(){
    return;
}



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
public:
numa():numa(10,0){}
numa(int sz, int val){
        data = new int[sz];
    }
int getData(int idx){
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
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        data = new int[sz];
    }
    int getData(int idx){
        return data[idx];
    }
    void setData(int idx, int value){
        data[idx] = value;
    }
};

#endif