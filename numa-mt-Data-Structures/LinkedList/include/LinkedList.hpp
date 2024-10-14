#ifndef _LINKEDLIST_HPP_
#define _LINKEDLIST_HPP_

#include "Node.hpp"

/*!
 * \class LinkedList
 *
 * \brief Interface for a simple LinkedList class.
 *
 * This is the interface for a simple linked list class.
 * The class utilizes the \class Node class as linked list objects.
 *
 */

#include<iostream>
using namespace std;

class LinkedList
{
private:
	Node *head;
	Node *tail;
	int length;


public:
	/*!
	 * \brief Default LinkedList Constructor
	 *
	 */
	LinkedList();

	/*!
	 * \brief LinkedList Destructor
	 *
	 * Need to remove each element one by one. There are two pointers maintained,
	 * the head Node and the tail Node. Must delete from head variable, while keeping track of
	 * next node so that we don't lose our LinkedList.  
	 * This can be done by calling our del function in a while loop.
	 *
	 * \sa LinkedList::del()
	 */
	~LinkedList();

	/*!
	 * \brief Function for removing a single LinkedList node
	 *
	 * The LinkedList::removeHead() function removes the least recently
	 * added node on the LinkedList. It also simultaneously updates the pointer
	 * to the head and tail of the LinkedList.
	 *
	 * \return The data from the removed node.
	 */
	int removeHead();

	/*!
	 * \brief append function for adding LinkedList variables to the end of the list
	 *
	 * The append function adds a new LinkedList node to the end of the LinkedList. 
	 *
	 * \param[in] data Data to be added to tail of LinkedList. This data is wrapped by a Node.
	 * 
	 * \note To avoid Memory Allocation issues, if allocation fails (i.e. overflow)
	 * 	LinkedList::prepend() is returned from immediately.
	 */

	void append(int data);

	/*!
	 * \brief prepend function for adding LinkedList variables to the beginning of the list
	 *
	 * The prepend function adds a new LinkedList node to the beginning of the LinkedList. 
	 *
	 * \param[in] data Data to be added to head of LinkedList. This data is wrapped by a Node.
	 * 
	 * \note To avoid Memory Allocation issues, if allocation fails (i.e. overflow)
	 * 	LinkedList::prepend() is returned from immediately.
	 */

	void prepend(int data);


	/*!
	 * \brief insertAfter function for adding LinkedList variables after a specific node
	 *
	 * The insertAfter function adds a new LinkedList node after the node with the specified
	 * data of the LinkedList. 
	 *
	 * \param[in] data Data to be added to head of LinkedList. This data is wrapped by a Node.
	 * 
	 * \note To avoid Memory Allocation issues, if allocation fails (i.e. overflow)
	 	LinkedList::insertAfter() is returned from immediately.
	 */
	void insertAfter(int existData, int newData);


	/*!
	 * \brief removeTail function for deleting the tail element of LinkedList
	 * 
	 * This function utilizes the tail member of the Linked List class to 
	 * remove the tail from the list.
	 *
	 * \return The value deleted from the list.
	 */

	int removeTail();


	/*!
	 * \brief insertAtIndex function for adding LinkedList variables after a specific node
	 *
	 * The insertAfter function adds a new LinkedList node with <b>newData</b> 
	 * after the node with the specified <b>index</b> of the LinkedList. 
	 *
	 * \param[in] data Data to be added to head of LinkedList. This data is wrapped by a Node.
	 * 
	 * \note To avoid Memory Allocation issues, if allocation fails (i.e. overflow)
	 	LinkedList::insertAfter() is returned from immediately.
	 */

	void insertAtIndex(int index, int newData);



	/*!
	 * \brief A function to display the contents of the LinkedList.
	 * 
	 * This function iterates over and prints each node in the LinkedList.
	 * The first node printed is the top Node.
	 */
	void display();

	/*!
	 * \brief A function to return the <b>length</b> of the list.
	 *
	 *
	 * \return The length of the Linked List.
	 *
	 */

	int getLength(){ return length; }

	


};

LinkedList::LinkedList()
{
	head = NULL;
	tail = NULL;
	length = 0;
}

LinkedList::~LinkedList()
{
	Node *temp = head;
	while(temp != NULL)
	{
		head = head->getLink();
		delete temp;
		temp = head;
		length--;
	}
	temp = NULL;
}

int LinkedList::removeHead()
{
	if(head == NULL)
	{
		std::cout << "LinkedList Empty!!" << std::endl;
		return -1;
	}
	Node *temp = head;
	head = head->getLink();
	int data = temp->getData();
	delete temp;
	temp = NULL;
	length--;

	return data;

}

void LinkedList::append(int data)
{
	if(head == NULL)
	{
		Node *newNode = new Node(data);
		newNode->setLink(NULL);
		head = newNode;
		length++;
		return;

	}

	Node *temp = head;
	while(true)
	{
		if(temp->getLink() == NULL)
		{
			Node *newNode = new Node(data);
			newNode->setLink(NULL);
			temp->setLink(newNode);
			length++;
			return;
		}
		temp = temp->getLink();
	}
}

void LinkedList::prepend(int data)
{
	Node *newNode = new Node(data);
	newNode->setLink(head);
	head = newNode;
	length++;
}

void LinkedList::insertAfter(int existData, int newData)
{
	if(head == NULL)
	{
		Node *newNode = new Node(newData);
		newNode->setLink(NULL);
		head = newNode;
		length++;
		return;
	}

	Node *temp = head;
	while(temp != NULL)
	{
		if(temp->getData() == existData)
		{
			Node *newNode = new Node(newData);
			newNode->setLink(temp->getLink());
			temp->setLink(newNode);
			length++;
			return;
		}
		temp = temp->getLink();
	}
}

int LinkedList::removeTail()
{
	/*
	 * Could use the length variable, but I want to implement
	 * it using pointers only.  Length is a debugging tool.
	 */
	if(head == NULL)
	{
		std::cout << "LinkedList Empty!!" << std::endl;
		return -1;
	}

	Node *current = head;
	Node *prev = NULL;
	while(current != NULL)
	{
		if(current->getLink() == NULL)
		{
			int data = current->getData();
			prev->setLink(NULL);
			delete current;
			length--;
			current = NULL;
			return data;
		}
		prev = current;
		current = current->getLink();
	}

	return -2;
}

void LinkedList::insertAtIndex(int index, int newData)
{
	if(index + 1 > length)
	{
		std::cout << "Index too large." << std::endl;
		return;
	}
	if(head == NULL)
	{
		Node *newNode = new Node(newData);
		newNode->setLink(NULL);
		head = newNode;
		length++;
		return;
	}

	Node *temp = head;
	Node *prev = NULL;
	int i = 0;
	while(temp != NULL)
	{
		if(i == index)
		{
			Node *newNode = new Node(newData);
			newNode->setLink(temp);
			prev->setLink(newNode);
			length++;
			return;
		}
		i++;
		prev = temp;
		temp = temp->getLink();
	}
}

void LinkedList::display()
{
	if(head == NULL)
	{
		std::cout << "LinkedList Empty!!" << std::endl;
		return;
	}

	Node *temp = head;
	while(temp != NULL)
	{
		if(temp == head)
		{
			std::cout << "HEAD ";;
		}
		std::cout << temp->getData() << std::endl;
		temp = temp->getLink();

	}
}





#endif //_LINKEDLIST_HPP_