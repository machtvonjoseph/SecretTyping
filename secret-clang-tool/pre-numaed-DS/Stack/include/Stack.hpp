/*! \file Stack.hpp
 * \brief Interface for a simple Stack class
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#ifndef _STACK_HPP_
#define _STACK_HPP_

#include "Node.hpp"
#include <iostream>

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

	Stack();

	~Stack();
	
	int pop();

	void push(int);

	void display();


};

Stack::Stack()
{
	top = nullptr;
}


Stack::~Stack()
{
	
	if(top == nullptr)
	{
		return;
	}
	Node *temp;
	while(top != nullptr)
	{
		temp = top;
		top = top->getLink();
		delete temp;
	}

}


int Stack::pop()
{
	if(top == nullptr)
	{
		return -1;
	}
	Node *retN = top;
	std::cout << "Popping: " << retN->getData() << std::endl;
	top = top->getLink();
	int data = retN->getData();
	delete retN;
	retN = nullptr;

	return data;
}


void Stack::push(int data)
{
	Node *newN = new Node(data);
	if(newN == nullptr)
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
	if(top == nullptr)
	{
		std::cout << "Stack Empty!!" << std::endl;
		return;
	}

	int i = 0;
	
	Node *temp = top;
	while (temp != nullptr)
	{
		
		if(i == 0)
			std::cout << "TOP ";

		std::cout << temp->getData() << std::endl;
		temp = temp->getLink();
		i++;
	}
}
#endif //_STACK_HPP_