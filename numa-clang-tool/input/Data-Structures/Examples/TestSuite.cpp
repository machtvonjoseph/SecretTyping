/*! \file TestSuite.cpp
 * \brief Testsuite implementation which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#include "TestSuite.hpp"
#include "Node.hpp"
// #include "Queue.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "BinarySearch.hpp"
#include "LinkedList.hpp"
// #include "LinkedList.hpp"
// #include "../src/Stack.cpp"
// #include "../src/Queue.cpp"
// #include "../src/BinarySearch.cpp"

// #include "../src/Node.cpp"
#include "numatype.hpp"

#include <iostream>

numa<Stack,0> *a;
numa<Queue,0> *b;
numa<BinarySearchTree,0> *c;
numa<LinkedList,0> *d;

void init(){
	a = new numa<Stack,0>();
	b = new numa<Queue,0>();
	c = new numa<BinarySearchTree,0>();
	d = new numa<LinkedList,0>();

}

void StackTest()
{
	Stack p;
	//Stack *b;
	cout<<"----------------------------------"<<endl;
	cout<<"NUMA Stack"<<endl;
	cout<<"----------------------------------"<<endl;

	a->push(1);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display NUMA Stack after adding 1 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a->display();
	

	a->push(5);
	a->push(30);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display NUMA Stack after adding 2 nodes." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a->display();

	a->pop();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display NUMA Stack after removing 1 node" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a->display();


	//b = new Stack();
	a->pop();
	a->pop();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display empty stack." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a->display();

	for(int i = 0; i < 5; i++)
	{
		a->push(i);
	}
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display NUMA Stack after adding 5 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a->display();

	for(int i = 0; i < 3; i++)
	{	
		a->pop();
	}

	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display NUMA Stack after removing 3 nodes." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a->display();

	delete a;
}

void QueueTest()
{
	Queue a;

	// b = new Queue();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display empty NUMA Queue." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	b->display();

	for(int i = 0; i < 5; i++)
	{
		b->add(i);
	}
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display NUMA Queue after adding 5 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	b->display();

	for(int i = 0; i < 3; i++)
	{	
		b->del();
	}

	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display NUMA Queue after removing 3 nodes." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	b->display();

	delete b;

}

void BinarySearchTest()
{
	BinarySearchTree a;
	c= new numa<BinarySearchTree,0>();

	cout<<"----------------------------------"<<endl;
	cout<<" NUMA Binary Search Tree"<<endl;
	cout<<"----------------------------------"<<endl;


	c->insert(4);
	c->insert(2);
	c->insert(5);
	c->insert(1);
	c->insert(3);
	

	c->postOrderPrint();
	c->preOrderPrint();
	c->inOrderPrint();

	delete c;
}

void LinkedListTest()
{
	LinkedList a;
	d = new numa<LinkedList,0>();
	cout<<"----------------------------------"<<endl;
	cout<<" NUMA Linked List"<<endl;
	cout<<"----------------------------------"<<endl;

	d->append(1);
	d->append(3);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display LinkedList after adding 2 nodes." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	d->display();

	d->append(20);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display LinkedList after adding 1 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	d->display();

	d->prepend(50);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Linked List after adding 1 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;

	
	d->display();

	std::cout << "----------------------------------" << std::endl;
	std::cout << "------------ LENGTH "<< a.getLength() << " --------------" << std::endl;
	std::cout << "----------------------------------" << std::endl;


	d->removeHead();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Linked List after removing HEAD node." << std::endl;
	std::cout << "----------------------------------" << std::endl;

	d->display();

	d->removeTail();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Linked List after removing TAIL node." << std::endl;
	std::cout << "----------------------------------" << std::endl;

	d->display();

	d->insertAfter(3,78);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Linked List after insertingAfter 1 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	d->display();

	d->insertAtIndex(1, 97);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Linked List after insertingAtIndex 1 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;

	d->display();

	for(int i = 0; i < 20; i++)
	{
		d->removeHead();
	}

	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Linked List after removing 20 nodes" << std::endl;
	std::cout << "----------------------------------" << std::endl;

	d->display();

}