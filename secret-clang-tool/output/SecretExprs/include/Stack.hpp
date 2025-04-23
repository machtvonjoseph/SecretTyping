#ifdef UMF 
	                #include "numatype.hpp"
	                #include <umf/mempolicy.h>
	                #include <umf/memspace.h>
                    #include "utils_examples.h"
                    #include <stdio.h>
                    #include <string.h>
                #endif
                #include "secrettype.hpp"
                /*! \file Stack.hpp
 * \brief Interface for a simple Stack class
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#ifndef _STACK_HPP_
#define _STACK_HPP_

#include "Node.hpp"
#include "iostream"	
#include "secrettype.hpp"
using namespace std;


/*!
 * \class Stack
 *
 * \brief Interface for a simple Stack class.
 *
 * This is the interface for a simple stack class.
 * The class utilizes the \class Node class as stack objects.
 * Thus, the stack is implemented as a Linked List.
 *
 */

class Stack
{

private:
	Node *top; //< Pointer to the top of the Stack


public:
	/*!
	 * \brief Default Stack Constructor
	 *
	 */
	Stack();

	/*!
	 * \brief Stack Destructor
	 *
	 * Need to remove each element one by one. The only pointer maintained
	 * is the top Node. Must delete top variable, while keeping track of
	 * next node so that we don't lose our stack.  
	 * This can be done by calling our pop function in a while loop.
	 *
	 * \sa Stack::pop()
	 */
	virtual ~Stack();

	/*!
	 * \brief Function for removing a single stack node
	 *
	 * The Stack::pop() function removes the most recently
	 * added node on the stack. It also simultaneously updates the pointer
	 * to the top of the stack.
	 *
	 * \return The data from the removed node.
	 */
	virtual int pop();

	/*!
	 * \brief Push function for adding stack variables
	 *
	 * The push function adds a new stack node to the top of the stack. 
	 *
	 * \param[in] data Data to be added to top of stack. This data is wrapped by a Node.
	 * 
	 * \note To avoid Memory Allocation issues, if allocation fails (i.e. overflow)
	 	Stack::push() is returned from immediately.
	 */
	virtual void push(int);


	/*!
	 * \brief A function to display the contents of the stack.
	 * 
	 * This function iterates over and prints each node in the stack.
	 * The first node printed is the top Node.
	 */
	virtual void display();


};

template<>
class secret<Stack>{
//add your secure memory allocator code herepublic:
secret (){
    this->top = __null;
}
virtual ~secret()
{
	
	if(top == NULL)
	{
		return;
	}
	Node *temp;
	while(top != NULL)
	{
		temp = top;
		top = top->getLink();
		delete temp;
	}

}
virtual int pop(){
    if (this->top == __null) {
        return -1;
    }
    Node *retN = this->top;
    this->top = this->top->getLink();
    int data = retN->getData();
    delete retN;
    retN = __null;
    return data;
}
virtual void push(int data){
    Node *newN = new Node(data);
    if (newN == __null) {
        std::cerr << "Stack full" << std::endl;
        return;
    }
    newN->setLink(this->top);
    this->top = newN;
}
virtual void display(){
    if (this->top == __null) {
        std::cout << "Stack Empty!!" << std::endl;
        return;
    }
    int i = 0;
    Node *temp = this->top;
    while (temp != __null)
        {
            if (i == 0)
                std::cout << "TOP ";
            std::cout << temp->getData() << std::endl;
            temp = temp->getLink();
            i++;
        }
}
private:
secret<Node*> top;
};

// template<>
// class secret<Stack>
// {
// };

// template<>
// class secret<Node>
// {
	
// };

Stack::Stack()
{
	top = NULL;
}


Stack::~Stack()
{
	
	if(top == NULL)
	{
		return;
	}
	Node *temp;
	while(top != NULL)
	{
		temp = top;
		top = top->getLink();
		delete temp;
	}

}


int Stack::pop()
{
	if(top == NULL)
	{
		return -1;
	}
	Node *retN = top;
	top = top->getLink();
	int data = retN->getData();
	delete retN;
	retN = NULL;
	return data;
}


void Stack::push(int data)
{
	Node *newN = new Node(data);
	if(newN == NULL)
	{
		std::cerr << "Stack full" << std::endl;
		return;
	}

	//Node *newNext = newN->getLink();
	newN->setLink(top);
	top = newN;

}

void Stack::display()
{
	if(top == NULL)
	{
		std::cout << "Stack Empty!!" << std::endl;
		return;
	}

	int i = 0;
	
	Node *temp = top;
	while (temp != NULL)
	{
		
		if(i == 0)
			std::cout << "TOP ";

		std::cout << temp->getData() << std::endl;
		temp = temp->getLink();
		i++;
	}
}


#endif //_STACK_HPP_