#include "numatype.hpp"
#ifndef _BINARYNODE_HPP_
#define _BINARYNODE_HPP_


#include <cstddef>



/*!
 * \class BinaryNode
 *
 * \brief Interface for a simple BinaryNode.
 *
 * This is the interface for a binary node. The binary
 * node inherits from a Node object.  Members added to 
 * the BinaryNode are isRoot, leftChild, rightChild
 *
 */

class BinaryNode //: public Node
{
private:
	//< Inherits int data from Node
	int data;
	BinaryNode *leftChild;
	BinaryNode *rightChild;

public:
	//< Inherits int getData() from Node 


	BinaryNode() : data(0), leftChild(NULL), rightChild(NULL)
	{}

	BinaryNode(int data) 
	: data(data), leftChild(NULL), rightChild(NULL)
	{}

	virtual ~BinaryNode()
	{
		
		leftChild = NULL;
		rightChild = NULL;
	}

	virtual int getData() { return data; }
	virtual void setData(int data) { data = data; }

	virtual BinaryNode *getLeftChild() { return leftChild; }
	virtual BinaryNode *getRightChild() { return rightChild; }

	virtual void setLeftChild(BinaryNode *node) { leftChild = node; }
	virtual void setRightChild(BinaryNode *node) { rightChild = node; }



};

template<>
class numa<BinaryNode,0>{
public: 
    static void* operator new(std::size_t sz){
        // cout<<"doing numa allocation \n";
		 void* p = numa_alloc_onnode(sz * sizeof(BinaryNode), 0);
        if (p == nullptr) {
            cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
		 void* p = numa_alloc_onnode(sz * sizeof(BinaryNode), 0);
        if (p == nullptr) {
            cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
        // cout<<"doing numa free \n";
		numa_free(ptr, 1 * sizeof(BinaryNode));
    }

    static void operator delete[](void* ptr){
		numa_free(ptr, 1 * sizeof(BinaryNode));
    }
public:
numa (): data(0), leftChild(__null), rightChild(__null){
}
numa (int data): data(data), leftChild(__null), rightChild(__null){
}
virtual ~numa()
{
		
		leftChild = NULL;
		rightChild = NULL;
	}
virtual int getData(){
    return this->data;
}
virtual void setData(int data){
    data = data;
}
virtual BinaryNode * getLeftChild(){
    return this->leftChild;
}
virtual BinaryNode * getRightChild(){
    return this->rightChild;
}
virtual void setLeftChild(BinaryNode * node){
    this->leftChild = node;
}
virtual void setRightChild(BinaryNode * node){
    this->rightChild = node;
}
private:
numa<int,0> data;
numa<BinaryNode*,0> leftChild;
numa<BinaryNode*,0> rightChild;
};

template<>
class numa<BinaryNode,1>{
public: 
    static void* operator new(std::size_t sz){
        // cout<<"doing numa allocation \n";
		 void* p = numa_alloc_onnode(sz * sizeof(BinaryNode), 1);
        if (p == nullptr) {
            cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
		 void* p = numa_alloc_onnode(sz * sizeof(BinaryNode), 1);
        if (p == nullptr) {
            cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
        // cout<<"doing numa free \n";
		numa_free(ptr, 1 * sizeof(BinaryNode));
    }

    static void operator delete[](void* ptr){
		numa_free(ptr, 1 * sizeof(BinaryNode));
    }
public:
numa (): data(0), leftChild(__null), rightChild(__null){
}
numa (int data): data(data), leftChild(__null), rightChild(__null){
}
virtual ~numa()
{
		
		leftChild = NULL;
		rightChild = NULL;
	}
virtual int getData(){
    return this->data;
}
virtual void setData(int data){
    data = data;
}
virtual BinaryNode * getLeftChild(){
    return this->leftChild;
}
virtual BinaryNode * getRightChild(){
    return this->rightChild;
}
virtual void setLeftChild(BinaryNode * node){
    this->leftChild = node;
}
virtual void setRightChild(BinaryNode * node){
    this->rightChild = node;
}
private:
numa<int,1> data;
numa<BinaryNode*,1> leftChild;
numa<BinaryNode*,1> rightChild;
};

#endif /* _BINARYNODE_HPP_ */