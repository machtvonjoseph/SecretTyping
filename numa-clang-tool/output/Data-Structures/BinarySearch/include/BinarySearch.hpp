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

};

template<>
class numa<BinarySearchTree,0>{
public: 
    static void* operator new(std::size_t sz){
        std::cout<<"new operator called"<<std::endl;
		 void* p = numa_alloc_onnode(sz * sizeof(BinarySearchTree), 0);
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    static void* operator new[](std::size_t sz){
		std::cout<<"new operator called"<<std::endl;
		 void* p = numa_alloc_onnode(sz * sizeof(BinarySearchTree), 0);
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }

    static void operator delete(void* ptr){
		std::cout<<"delete operator called"<<std::endl;
		numa_free(ptr, 1 * sizeof(BinarySearchTree));
    }

    static void operator delete[](void* ptr){
		std::cout<<"delete operator called"<<std::endl;
		numa_free(ptr, 1 * sizeof(BinarySearchTree));
    }
public:
numa (){
}
virtual ~numa()
{
	root = NULL;
}
virtual void insert(int data){
    BinaryNode *leaf = new BinaryNode(data);
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
    if (leaf->getData() <= parent->getData())
        parent->setLeftChild(leaf);
    else
        parent->setRightChild(leaf);
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
private:
numa<BinaryNode*,0> root;
};

BinarySearchTree::BinarySearchTree() : root(NULL)
{
	
}

BinarySearchTree::~BinarySearchTree()
{
	root = NULL;
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
	if(leaf->getData() <= parent->getData())
		parent->setLeftChild(leaf);
	else
		parent->setRightChild(leaf);

	
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

#endif //_BINARYSEARCH_HPP_