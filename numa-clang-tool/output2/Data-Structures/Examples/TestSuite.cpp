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
// #include "BinarySearch.hpp"
// #include "LinkedList.hpp"
// #include "../src/Stack.cpp"
// #include "../src/Queue.cpp"
// #include "../src/BinarySearch.cpp"
// #include "../src/LinkedList.cpp"
// #include "../src/Node.cpp"
#include "numatype.hpp"

#include <iostream>


void StackTest()
{
	Stack a;
	numa<Stack,0>* v = new numa<Stack,0>();
	//Stack *b;
	v->push(1);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Stack after adding 1 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	v->display();
	

	v->push(5);
	v->push(30);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Stack after adding 2 nodes." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	v->display();

	v->pop();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Stack after removing 1 node" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	v->display();


	//b = new Stack();
	v->pop();
	v->pop();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display empty stack." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	v->display();

	for(int i = 0; i < 5; i++)
	{
		v->push(i);
	}
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Stack after adding 5 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	v->display();

	for(int i = 0; i < 3; i++)
	{	
		v->pop();
	}

	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Stack after removing 3 nodes." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	v->display();

	delete v;
}

void QueueTest()
{
	Queue a;
	numa<Queue,0> *b = new numa<Queue,0>();

	a.add(1);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Queue after adding 1 node." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a.display();
	

	a.add(5);
	a.add(30);
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Queue after adding 2 nodes." << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a.display();

	a.del();
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Display Queue after removing 1 node" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	a.display();

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

// void BinarySearchTest()
// {
// 	BinarySearchTree a;
// 	BinarySearchTree *b;
// 	a.insert(4);
// 	a.insert(2);
// 	a.insert(5);
// 	a.insert(1);
// 	a.insert(3);
	

// 	a.postOrderPrint();
// 	a.preOrderPrint();
// 	a.inOrderPrint();

// }

// void LinkedListTest()
// {
// 	LinkedList a;
// 	LinkedList *b;

// 	a.append(1);
// 	a.append(3);
// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display LinkedList after adding 2 nodes." << std::endl;
// 	std::cout << "----------------------------------" << std::endl;
// 	a.display();

// 	a.append(20);
// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display LinkedList after adding 1 node." << std::endl;
// 	std::cout << "----------------------------------" << std::endl;
// 	a.display();

// 	a.prepend(50);
// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display Linked List after adding 1 node." << std::endl;
// 	std::cout << "----------------------------------" << std::endl;

	
// 	a.display();

// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "------------ LENGTH "<< a.getLength() << " --------------" << std::endl;
// 	std::cout << "----------------------------------" << std::endl;


// 	a.removeHead();
// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display Linked List after removing HEAD node." << std::endl;
// 	std::cout << "----------------------------------" << std::endl;

// 	a.display();

// 	a.removeTail();
// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display Linked List after removing TAIL node." << std::endl;
// 	std::cout << "----------------------------------" << std::endl;

// 	a.display();

// 	a.insertAfter(3,78);
// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display Linked List after insertingAfter 1 node." << std::endl;
// 	std::cout << "----------------------------------" << std::endl;
// 	a.display();

// 	a.insertAtIndex(1, 97);
// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display Linked List after insertingAtIndex 1 node." << std::endl;
// 	std::cout << "----------------------------------" << std::endl;

// 	a.display();

// 	for(int i = 0; i < 20; i++)
// 	{
// 		a.removeHead();
// 	}

// 	std::cout << "----------------------------------" << std::endl;
// 	std::cout << "Display Linked List after removing 20 nodes" << std::endl;
// 	std::cout << "----------------------------------" << std::endl;

// 	a.display();
// }