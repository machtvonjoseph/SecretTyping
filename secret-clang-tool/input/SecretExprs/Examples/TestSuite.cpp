/*! \file TestSuite.cpp
 * \brief Testsuite implementation which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#include "TestSuite.hpp"
#include "Node.hpp"
#include "Stack.hpp"
\
// #include "numatype.hpp"
#include <random>
#include <iostream>
#include <thread>

#include <mutex>
#include <syncstream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include <map>
#include <atomic>


using namespace std::chrono;
secret<Stack>* secretStack;
Stack* stack;

secret<Node>* secretNode;
Node* node;

void secret_Stack_init(){
	secretStack = new secret<Stack>();
	stack = new Stack();
	secretNode = new secret<Node>();
	node = new Node();
	secret<Stack> inFunctionSecretStack;
	Stack inFunctionStack;
	secret<Node> inFunctionSecretNode;
	Node inFunctionNode;

	secret<Stack>* secretDefinition = new secret<Stack>();
	Stack* stackDefinition = new Stack();

	secret<Node>* secretNodeDefinition = new secret<Node>();
	Node* nodeDefinition = new Node();
}

void StackTest()
{	

}
