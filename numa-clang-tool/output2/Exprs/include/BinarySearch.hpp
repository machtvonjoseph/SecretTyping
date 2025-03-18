#ifdef UMF 
	                #include "numatype.hpp"
	                #include <umf/mempolicy.h>
	                #include <umf/memspace.h>
                    #include "utils_examples.h"
                    #include "umf_numa_allocator.hpp"
                    #include <numa.h>
                    #include <numaif.h>
                    #include <stdio.h>
                    #include <string.h>
                #endif
                #include "numatype.hpp"
                #ifndef _BINARYSEARCH_HPP_
#define _BINARYSEARCH_HPP_



//#include "BinaryNode.hpp"

#include <iostream>
#include "BinaryNode.hpp"
using namespace std;
class BinaryNode;

/*!
 * \class BinarySearchTree
 *
 * \brief Interface for a simple BinarySearchTree class.
 *
 * This is the interface for a simple binary search tree class.
 * The class utilizes the \class Node class as leaves in the tree.
 *
 */

class BinarySearchTree
{
private:
	BinaryNode *root;
	

public:
	/*!
	 * \brief BinarySearchTree Constructor
	 *
	 */

	BinarySearchTree();

	/*!
	 * \brief BinarySearchTree Destructor
	 *
	 */
	virtual ~BinarySearchTree();

	/*!
	 * \brief Insert data into the binary search tree
	 * 
	 * This method allows us to insert data into the BST via
	 * recursion.
	 *
	 *
	 * \param[in] data Data to be inserted.
	 *
	 */
	virtual BinaryNode* deleteHelper(BinaryNode *node, int data);

	virtual BinaryNode* findMin(BinaryNode *node);

	virtual void insert(int data);

	/*!
	 * \brief Look up data in the binary search tree
	 * 
	 * This method allows us determine if a specific
	 * piece of data is in the tree.
	 *
	 * \return True if the data is in the tree, else false.
	 *
	 */ 

	virtual bool lookup(int data);

	/*!
	 * 
	 * \brief Post order print method to display the tree
	 *
	 * Transverse the left leaf in postorder.
	 * Transverse the right leaf in postorder.
	 * Visit the root.
	 *
	 */


	virtual void postOrderPrint();

	/*! 
	 * \brief Pre order print method to display the tree
	 *
	 * Visit the root.
	 * Transverse the left leaf in preorder.
	 * Transverse the right leaf in preorder.
	 *
	 * 
	 */
	
	virtual void preOrderPrint();

	/*!
	 * \brief In order print method to display the tree
	 * 
	 * Transverse the left leaf in inorder.
	 * Visit the root.
	 * Transverse the right leaf in inorder.
	 *
	 */
	virtual void inOrderPrint();

	//More functions to come soon


	//Recursive Functions
	virtual void inOrder(BinaryNode *node);

	virtual void preOrder(BinaryNode *node);

	virtual void postOrder(BinaryNode *node);

	virtual void update(int data);

	virtual void remove(int data);

};

template<>
class numa<BinarySearchTree,0>{
public: 
    static void* operator new(std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(0 ,sizeof(BinarySearchTree),alignof(BinarySearchTree));
        #else
            p = numa_alloc_onnode(sz* sizeof(BinarySearchTree), 0);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(0 ,sizeof(BinarySearchTree),alignof(BinarySearchTree));
        #else
            p = numa_alloc_onnode(sz* sizeof(BinarySearchTree), 0);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
        // cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(0,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(BinarySearchTree));
        #endif
    }

    static void operator delete[](void* ptr){
		// cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(0,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(BinarySearchTree));
        #endif
    }
public:
numa (){
}
virtual ~numa()
{
	root = NULL;
}
virtual BinaryNode * deleteHelper(BinaryNode * node, int key){
    if (!node)
        return nullptr;
    if (key < node->getData()) {
        node->setLeftChild(this->deleteHelper(node->getLeftChild(), key));
    } else if (key > node->getData()) {
        node->setRightChild(this->deleteHelper(node->getLeftChild(), key));
    } else {
        if (!node->getLeftChild()) {
            BinaryNode *temp = node->getRightChild();
            delete node;
            return temp;
        } else if (!node->getRightChild()) {
            BinaryNode *temp = node->getLeftChild();
            delete node;
            return temp;
        }
        BinaryNode *temp = this->findMin(node->getRightChild());
        node->setData(temp->getData());
        node->setRightChild(this->deleteHelper(node->getRightChild(), temp->getData()));
    }
    return node;
}
virtual BinaryNode * findMin(BinaryNode * node){
    while (node && node->getLeftChild())
        node = node->getLeftChild();
    return node;
}
virtual void insert(int data){
    BinaryNode *leaf = reinterpret_cast<BinaryNode*>(new numa<BinaryNode,0>(data));
    if (this->root == __null) {
        this->root = leaf;
        return;
    }
    BinaryNode *current = this->root;
    BinaryNode *parent;
    while (current != __null)
        {
            parent = current;
            if (data <= current->getData()) {
                current = current->getLeftChild();
            } else {
                current = current->getRightChild();
            }
        }
    if (leaf->getData() < parent->getData())
        parent->setLeftChild(leaf);
    else if (leaf->getData() > parent->getData())
        parent->setRightChild(leaf);
    else
        delete leaf;
}
virtual bool lookup(int data){
    if (this->root == __null) {
        return false;
    }
    BinaryNode *current = this->root;
    BinaryNode *parent;
    while (current != __null)
        {
            if (current->getData() == data) {
                return true;
            }
            if (data < current->getData()) {
                current = current->getLeftChild();
            } else {
                current = current->getRightChild();
            }
        }
    return false;
}
virtual void postOrderPrint(){
    std::cout << "Post Order Print" << std::endl;
    this->postOrder(this->root);
    std::cout << std::endl;
}
virtual void preOrderPrint(){
    std::cout << "Pre Order Print" << std::endl;
    this->preOrder(this->root);
    std::cout << std::endl;
}
virtual void inOrderPrint(){
    std::cout << "In Order Print" << std::endl;
    this->inOrder(this->root);
    std::cout << std::endl;
}
virtual void inOrder(BinaryNode * node){
    if (node == __null)
        return;
    if (node->getLeftChild() != __null)
        this->inOrder(node->getLeftChild());
    std::cout << " " << node->getData() << " ";
    if (node->getRightChild() != __null)
        this->inOrder(node->getRightChild());
}
virtual void preOrder(BinaryNode * node){
    if (node == __null)
        return;
    std::cout << " " << node->getData() << " ";
    if (node->getLeftChild() != __null)
        this->inOrder(node->getLeftChild());
    if (node->getRightChild() != __null)
        this->inOrder(node->getRightChild());
}
virtual void postOrder(BinaryNode * node){
    if (node == __null)
        return;
    if (node->getLeftChild() != __null)
        this->postOrder(node->getLeftChild());
    if (node->getRightChild() != __null)
        this->postOrder(node->getRightChild());
    std::cout << " " << node->getData() << " ";
}
virtual void update(int data){
    if (this->root == __null) {
        return;
    }
    BinaryNode *current = this->root;
    BinaryNode *parent;
    while (current != __null)
        {
            if (current->getData() == data) {
                return;
            }
            if (data < current->getData()) {
                current = current->getLeftChild();
            } else {
                current = current->getRightChild();
            }
        }
}
virtual void remove(int data){
    this->root = this->deleteHelper(this->root, data);
}
private:
numa<BinaryNode*,0> root;
};

template<>
class numa<BinarySearchTree,1>{
public: 
    static void* operator new(std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(1 ,sizeof(BinarySearchTree),alignof(BinarySearchTree));
        #else
            p = numa_alloc_onnode(sz* sizeof(BinarySearchTree), 1);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
        void* p;
        #ifdef UMF
            p= umf_alloc(1 ,sizeof(BinarySearchTree),alignof(BinarySearchTree));
        #else
            p = numa_alloc_onnode(sz* sizeof(BinarySearchTree), 1);
        #endif
        
        if (p == nullptr) {
            std::cout<<"allocation failed\n";
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
        // cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(1,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(BinarySearchTree));
        #endif
    }

    static void operator delete[](void* ptr){
		// cout<<"doing numa free \n";
        #ifdef UMF
			umf_free(1,ptr);
		#else
		    numa_free(ptr, 1 * sizeof(BinarySearchTree));
        #endif
    }
public:
numa (){
}
virtual ~numa()
{
	root = NULL;
}
virtual BinaryNode * deleteHelper(BinaryNode * node, int key){
    if (!node)
        return nullptr;
    if (key < node->getData()) {
        node->setLeftChild(this->deleteHelper(node->getLeftChild(), key));
    } else if (key > node->getData()) {
        node->setRightChild(this->deleteHelper(node->getLeftChild(), key));
    } else {
        if (!node->getLeftChild()) {
            BinaryNode *temp = node->getRightChild();
            delete node;
            return temp;
        } else if (!node->getRightChild()) {
            BinaryNode *temp = node->getLeftChild();
            delete node;
            return temp;
        }
        BinaryNode *temp = this->findMin(node->getRightChild());
        node->setData(temp->getData());
        node->setRightChild(this->deleteHelper(node->getRightChild(), temp->getData()));
    }
    return node;
}
virtual BinaryNode * findMin(BinaryNode * node){
    while (node && node->getLeftChild())
        node = node->getLeftChild();
    return node;
}
virtual void insert(int data){
    BinaryNode *leaf = reinterpret_cast<BinaryNode*>(new numa<BinaryNode,1>(data));
    if (this->root == __null) {
        this->root = leaf;
        return;
    }
    BinaryNode *current = this->root;
    BinaryNode *parent;
    while (current != __null)
        {
            parent = current;
            if (data <= current->getData()) {
                current = current->getLeftChild();
            } else {
                current = current->getRightChild();
            }
        }
    if (leaf->getData() < parent->getData())
        parent->setLeftChild(leaf);
    else if (leaf->getData() > parent->getData())
        parent->setRightChild(leaf);
    else
        delete leaf;
}
virtual bool lookup(int data){
    if (this->root == __null) {
        return false;
    }
    BinaryNode *current = this->root;
    BinaryNode *parent;
    while (current != __null)
        {
            if (current->getData() == data) {
                return true;
            }
            if (data < current->getData()) {
                current = current->getLeftChild();
            } else {
                current = current->getRightChild();
            }
        }
    return false;
}
virtual void postOrderPrint(){
    std::cout << "Post Order Print" << std::endl;
    this->postOrder(this->root);
    std::cout << std::endl;
}
virtual void preOrderPrint(){
    std::cout << "Pre Order Print" << std::endl;
    this->preOrder(this->root);
    std::cout << std::endl;
}
virtual void inOrderPrint(){
    std::cout << "In Order Print" << std::endl;
    this->inOrder(this->root);
    std::cout << std::endl;
}
virtual void inOrder(BinaryNode * node){
    if (node == __null)
        return;
    if (node->getLeftChild() != __null)
        this->inOrder(node->getLeftChild());
    std::cout << " " << node->getData() << " ";
    if (node->getRightChild() != __null)
        this->inOrder(node->getRightChild());
}
virtual void preOrder(BinaryNode * node){
    if (node == __null)
        return;
    std::cout << " " << node->getData() << " ";
    if (node->getLeftChild() != __null)
        this->inOrder(node->getLeftChild());
    if (node->getRightChild() != __null)
        this->inOrder(node->getRightChild());
}
virtual void postOrder(BinaryNode * node){
    if (node == __null)
        return;
    if (node->getLeftChild() != __null)
        this->postOrder(node->getLeftChild());
    if (node->getRightChild() != __null)
        this->postOrder(node->getRightChild());
    std::cout << " " << node->getData() << " ";
}
virtual void update(int data){
    if (this->root == __null) {
        return;
    }
    BinaryNode *current = this->root;
    BinaryNode *parent;
    while (current != __null)
        {
            if (current->getData() == data) {
                return;
            }
            if (data < current->getData()) {
                current = current->getLeftChild();
            } else {
                current = current->getRightChild();
            }
        }
}
virtual void remove(int data){
    this->root = this->deleteHelper(this->root, data);
}
private:
numa<BinaryNode*,1> root;
};

BinarySearchTree::BinarySearchTree() : root(NULL)
{
	
}

BinarySearchTree::~BinarySearchTree()
{
	root = NULL;
}

void BinarySearchTree::update(int data)
{
	if(root == NULL)
	{
		return;
	}
	BinaryNode *current = root;
	BinaryNode *parent;
	while(current != NULL)
	{
		if(current->getData() == data)
		{
			return;
		}
		if(data < current->getData())
		{
			current = current->getLeftChild();
		}else
		{
			current = current->getRightChild();
		}
	}
}

void BinarySearchTree::remove(int data)
{
	root = deleteHelper(root, data);
	// if(root == NULL)
	// {
	// 	return;
	// }
	// BinaryNode *current = root;
	// BinaryNode *parent = NULL;
	// bool isLeft = false;
	// while(current != NULL)
	// {
	// 	if(current->getData() == data)
	// 	{
	// 		if(parent == NULL){
	// 			delete current;
	// 			root = NULL;
	// 			return;
	// 		}

	// 		if(isLeft){
	// 			BinaryNode *replacement = current->getRightChild();
	// 			parent->setLeftChild(replacement);
	// 			replacement->setLeftChild(current->getLeftChild());
	// 			delete current;
	// 		}
	// 		else{
	// 			BinaryNode *replacement = current->getLeftChild();
	// 			parent->setRightChild(replacement);
	// 			replacement->setRightChild(current->getRightChild());
	// 			delete current;
	// 		}
	// 		return;
	// 	}
	// 	if(data < current->getData())
	// 	{
	// 		parent = current;
	// 		current = current->getLeftChild();
	// 		isLeft = true;
	// 	}else
	// 	{
	// 		parent = current;
	// 		current = current->getRightChild();
	// 		isLeft = false;
	// 	}
	// }
}

void BinarySearchTree::insert(int data)
{
	BinaryNode *leaf = new BinaryNode(data);
	if(root == NULL)
	{
		root = leaf;
		return;
	}
	BinaryNode *current = root;
	BinaryNode *parent;
	while(current != NULL)
	{
		parent = current;
		if(data <= current->getData())
		{
			current = current->getLeftChild();
			
		}else
		{
			current = current->getRightChild();
			
		}
	}

	//current = leaf;
	if(leaf->getData() < parent->getData())
		parent->setLeftChild(leaf);
	else if(leaf->getData() > parent->getData())
		parent->setRightChild(leaf);
	else
		delete leaf;
}


bool BinarySearchTree::lookup(int data)
{
	if (root == NULL)
	{
		return false;
	}
	BinaryNode *current = root;
	BinaryNode *parent;
	while (current != NULL)
	{
		if (current->getData() == data)
		{
			return true;
		}

		if(data < current->getData())
		{
			current = current->getLeftChild();
		}else{
			current = current->getRightChild();
		}
	}

	return false;
}

void BinarySearchTree::postOrderPrint()
{
	std::cout << "Post Order Print" << std::endl;
	postOrder(root);
	std::cout << std::endl;

}

void BinarySearchTree::postOrder(BinaryNode *node)
{
	if(node == NULL)
		return;
	if(node->getLeftChild() != NULL)
		postOrder(node->getLeftChild());
	if(node->getRightChild() != NULL)
		postOrder(node->getRightChild());

	std::cout << " " << node->getData() << " ";

}

void BinarySearchTree::preOrderPrint()
{
	std::cout << "Pre Order Print" << std::endl;
	preOrder(root);
	std::cout << std::endl;

}

void BinarySearchTree::preOrder(BinaryNode *node)
{
	if (node == NULL)
		return;
	
	std::cout << " " << node->getData() << " ";

	if (node->getLeftChild() != NULL)
		inOrder(node->getLeftChild());	

	if (node->getRightChild() != NULL)
		inOrder(node->getRightChild());
}

void BinarySearchTree::inOrderPrint()
{
	std::cout << "In Order Print" << std::endl;
	inOrder(root);
	std::cout << std::endl;

}

void BinarySearchTree::inOrder(BinaryNode *node)
{
	if (node == NULL)
		return;
	
	
	if (node->getLeftChild() != NULL)
		inOrder(node->getLeftChild());	

	std::cout << " " << node->getData() << " ";

	if (node->getRightChild() != NULL)
		inOrder(node->getRightChild());	

}


BinaryNode* BinarySearchTree::findMin(BinaryNode *node){
		while (node && node->getLeftChild())
            node = node->getLeftChild();
        return node;
}


BinaryNode* BinarySearchTree::deleteHelper(BinaryNode *node, int key){
	if (!node) return nullptr; // Key not found

    if (key < node->getData()) {
        node->setLeftChild(deleteHelper(node->getLeftChild(), key));
    } 
	else if (key > node->getData()) {
        node->setRightChild( deleteHelper(node->getLeftChild(), key));
    } else {
         // Node found, handle deletion cases
        if (!node->getLeftChild()) {
			BinaryNode* temp = node->getRightChild();
			delete node;
			return temp;
        } 
		else if (!node->getRightChild()) {
			BinaryNode* temp = node->getLeftChild();
			delete node;
			return temp;
        }

            // Two children: Replace with inorder successor
            BinaryNode* temp = findMin(node->getRightChild());
			node->setData(temp->getData());
			node->setRightChild(deleteHelper(node->getRightChild(), temp->getData()));
        }
        return node;
}

#endif //_BINARYSEARCH_HPP_