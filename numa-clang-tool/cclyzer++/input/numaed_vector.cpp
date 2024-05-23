#include"numatype.hpp"



class MyVector;
template<typename T, int NodeID>
class numa<T , NodeID, NumaAllocator, typename std::enable_if<std::is_same<T, MyVector>::value>::type>{
using allocator_type = NumaAllocator<T,NodeID>;// Alloc<T, NodeID>;
using pointer_alloc_type =NumaAllocator<T*,NodeID>; //Alloc<T*, NodeID>;
public:
    numa(){
        allocator_type alloc;
        T* p = alloc.allocate(1);
        alloc.construct(p, T());                //If possible, construct an object of type T in allocated unititialized storage pointed to by p
                                                //but in our case we are only handling objects with no member functions(??)
        }


    template<typename... Args>
    numa(Args&&... args) {
        allocator_type alloc;
        T* p = alloc.allocate(1);
        alloc.construct(p, T(std::forward<Args>(args)...));                //If possible, construct an object of type T in allocated unititialized storage pointed to by p
                                                //but in our case we are only handling objects with no member functions(??)
        }

    numa(size_t size){                          //This is to allocate a custom size. Like in a vector
        allocator_type alloc;
        T* p = alloc.allocate(size);
        alloc.construct(p, T());                
    }
    explicit numa(const allocator_type& alloc) noexcept : T(alloc) {}
    explicit numa(const T& t, const allocator_type& alloc = allocator_type()) : T(t, alloc) {}
    explicit numa(T&& t, const allocator_type& alloc = allocator_type()) : T(std::move(t), alloc) {}    
    
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

    // assignment operators, use NumaAllocator
    numa& operator=(const T& t) {
        allocator_type alloc;
        T* p = alloc.allocate(1);       //1 * sizeof(T) bytes allocated
        alloc.construct(p, t);      //construct then swap
        this->swap(*p);
        return *this;
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
numa<int *, 3> my_data;

public:
//numa():numa(10,0){}
numa(int sz, int val){
        //my_data = new int[sz];
        int* alias = my_data;
        int* alias2 = my_data;
    }
int getData(int idx){

        return my_data[idx];
    }
void setData(int idx, int value){
        my_data[idx] = value;
    }
void my_func(){
        
    }
void ur_func(){
        
    }
};
class MyVector{  
    public:
    int* my_data;
public:
    MyVector():MyVector(10,0){};
    MyVector(int sz, int val = 0){
        my_data = new int[sz];
        int* alias = my_data;
        int* alias2 = my_data;
    }
    int getData(int idx){
        return my_data[idx];
    }
    void setData(int idx, int value){
        my_data[idx] = value;
    }
};

int main(){    
    numa<int, 3>* v2 = new numa<int, 3>();        
    numa<MyVector,3>* v = new numa<MyVector,3>(10,3);   
    MyVector* v3 = new MyVector(10,3);
    //v->my_data = new numa<int,3>();                      //Idk what the problem is here
//must support v= new MyVector(10)        AST handling is different
}