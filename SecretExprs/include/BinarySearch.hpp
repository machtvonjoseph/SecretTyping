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
	~BinarySearchTree();

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
	BinaryNode* deleteHelper(BinaryNode* parent, BinaryNode *node, int data);

	BinaryNode* findMin(BinaryNode *node);

	int insert(int data);

	int getDepth();
	int getDepthHelper(BinaryNode *node);
	/*!
	 * \brief Look up data in the binary search tree
	 * 
	 * This method allows us determine if a specific
	 * piece of data is in the tree.
	 *
	 * \return True if the data is in the tree, else false.
	 *
	 */ 

	int lookup(int data);

	/*!
	 * 
	 * \brief Post order print method to display the tree
	 *
	 * Transverse the left leaf in postorder.
	 * Transverse the right leaf in postorder.
	 * Visit the root.
	 *
	 */


	void postOrderPrint();

	/*! 
	 * \brief Pre order print method to display the tree
	 *
	 * Visit the root.
	 * Transverse the left leaf in preorder.
	 * Transverse the right leaf in preorder.
	 *
	 * 
	 */
	
	void preOrderPrint();

	/*!
	 * \brief In order print method to display the tree
	 * 
	 * Transverse the left leaf in inorder.
	 * Visit the root.
	 * Transverse the right leaf in inorder.
	 *
	 */
	void inOrderPrint();

	//More functions to come soon


	//Recursive Functions
	void inOrder(BinaryNode *node);

	void preOrder(BinaryNode *node);

	void postOrder(BinaryNode *node);

	void update(int data);

	void remove(int data);

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
	if(root==NULL){return;}
	if(root->getData() == data)
	{
		delete root;
		root = NULL;
		return;
	}
	else if (root->getData() < data) {
        root->setLeftChild(deleteHelper(root, root->getLeftChild(), data));
    } 
	else if (data > root->getData()) {
        root->setRightChild( deleteHelper(root, root->getRightChild(), data));
    }


	//root = deleteHelper(root, data);
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

int BinarySearchTree::insert(int data)
{
	BinaryNode *leaf = new BinaryNode(data);
	if(root == NULL)
	{
		root = leaf;
		return 0;
	}
	BinaryNode *current = root;
	BinaryNode *parent= nullptr;
	int level = 0;
	while(current != NULL)
	{
		parent = current;
		if(data < current->getData())
		{
			current = current->getLeftChild();
			level++;
			
		}else if(data > current->getData())
		{
			current = current->getRightChild();
			level++;
			
		}
		else{
			//std::cout << data << " already exists at level " << level << std::endl;
			delete leaf;
			return level;
		}
	}

	//current = leaf;
	if(data < parent->getData()){
		parent->setLeftChild(leaf);
	}
	else {
		parent->setRightChild(leaf);
	}
	//std::cout << "Inserted " << data << " at level " << level << std::endl;
	return level;
}


int BinarySearchTree::getDepth()
{
	return getDepthHelper(root);
	
}

int BinarySearchTree::getDepthHelper(BinaryNode *node)
{
	if(node == NULL)
		return 0;
	
	int leftDepth = getDepthHelper(node->getLeftChild());
	int rightDepth = getDepthHelper(node->getRightChild());
	return std::max(leftDepth, rightDepth) + 1;
	
}

int BinarySearchTree::lookup(int data)
{
	if (root == NULL)
	{
		return 0;
	}
	BinaryNode *current = root;
	BinaryNode *parent;
	int level = 0;
	while (current != NULL)
	{
		if (current->getData() == data)
		{
			//std::cout << "Found " << data << " at level " << level << std::endl;
			return level;
		}

		if(data < current->getData())
		{
			current = current->getLeftChild();
			level++;
		}else{
			current = current->getRightChild();
			level++;
		}
	}
	//std::cout << data << " not found. Searched up to depth " << level << std::endl;

	return level;
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


BinaryNode* BinarySearchTree::deleteHelper(BinaryNode* parent, BinaryNode *node, int key){
	if (!node) return nullptr; // Key not found

    if (key < node->getData()) {
        node->setLeftChild(deleteHelper(node, node->getLeftChild(), key));
    } 
	else if (key > node->getData()) {
        node->setRightChild( deleteHelper(node, node->getRightChild(), key));
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
			BinaryNode* temp2 = new BinaryNode(temp->getData());
			temp2->setLeftChild(node->getLeftChild());
			temp2->setRightChild(node->getRightChild());
			delete node;
			//node->setData(temp->getData());
			if(parent->getData() > temp2->getData())
			{
				parent->setLeftChild(temp2);
			}
			else
			{
				parent->setRightChild(temp2);
			}
			node->setRightChild(deleteHelper(temp2, temp2->getRightChild(), temp2->getData()));
        }
        return node;
}

#endif //_BINARYSEARCH_HPP_