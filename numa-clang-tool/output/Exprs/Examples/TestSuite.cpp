/*! \file TestSuite.cpp
 * \brief Testsuite implementation which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#include "TestSuite.hpp"
#include "Node.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "BinarySearch.hpp"
#include "LinkedList.hpp"
#include "numatype.hpp"
#include <random>
#include <iostream>
#include <thread>
// #include <barrier>
#include <mutex>
#include <syncstream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include <map>

std::vector<numa<Stack,0>*> Stacks0;
std::vector<numa<Stack,1>*> Stacks1;
int64_t StackOps0;
int64_t StackOps1;
mutex* Stack_lk0;
mutex* Stack_lk1;
pthread_barrier_t StackBar;

std::vector<numa<Queue,0>*> Queues0;
std::vector<numa<Queue,1>*> Queues1;
int64_t QueueOps0;
int64_t QueueOps1;
mutex* Queue_lk0;
mutex* Queue_lk1;
pthread_barrier_t QueueBar;


std::vector<numa<BinarySearchTree,0>*> BSTs0;
std::vector<numa<BinarySearchTree,1>*> BSTs1;
int64_t BSTOps0;
int64_t BSTOps1;
mutex* BST_lk0;
mutex* BST_lk1;
pthread_barrier_t BSTBar;

std::vector<numa<LinkedList,0>*> LLs0;
std::vector<numa<LinkedList,1>*> LLs1;
int64_t LLOps0;
int64_t LLOps1;
mutex* LL_lk0;
mutex* LL_lk1;
pthread_barrier_t LLBar;



// chrono::high_resolution_clock::time_point startTimer;
// chrono::high_resolution_clock::time_point endTimer;




void numa_Stack_init(int num_DS, int num_threads){
	Stacks0.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		Stacks0[i] = new numa<Stack,0>();
	}
	
	std::vector<numa<Stack,1>*> Stacks1;
	Stacks1.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		Stacks1[i] = new numa<Stack,1>();
	}

	Stack_lk0 = new mutex();
	Stack_lk1 = new mutex();
	pthread_barrier_init(&StackBar, NULL, num_threads);
	StackOps0 = 0;
	StackOps1 = 0;
	
}

void numa_Queue_init(int num_DS){
	std::vector<numa<Queue,0>*> Queues0;
	Queues0.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		Queues0[i] = new numa<Queue,0>();
	}
	
	std::vector<numa<Queue,1>*> Queues1;
	Queues1.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		Queues1[i] = new numa<Queue,1>();
	}

	Queue_lk0 = new mutex();
	Queue_lk1 = new mutex();
}

void numa_BST_init(int num_DS){
	std::vector<numa<BinarySearchTree,0>*> BSTs0;
	BSTs0.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		BSTs0[i] = new numa<BinarySearchTree,0>();
	}
	
	std::vector<numa<BinarySearchTree,1>*> BSTs1;
	BSTs1.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		BSTs1[i] = new numa<BinarySearchTree,1>();
	}

	BST_lk0 = new mutex();
	BST_lk1 = new mutex();
}

void numa_LL_init(int num_DS){
	std::vector<numa<LinkedList,0>*> LLs0;
	LLs0.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		LLs0[i] = new numa<LinkedList,0>();
	}
	
	std::vector<numa<LinkedList,1>*> LLs1;
	LLs1.resize(num_DS/2);
	for(int i = 0; i < num_DS/2; i++)
	{
		LLs1[i] = new numa<LinkedList,1>();
	}

	LL_lk0 = new mutex();
	LL_lk1 = new mutex();
}


void* StackTest(int tid, int duration, int node)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	pthread_barrier_wait(&StackBar);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

	if(node == 0){
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < Stacks0.size(); i++)
				{
					Stack_lk0->lock();
					Stacks0[i]->push(1);
					StackOps0++;
					Stack_lk0->unlock();
				}
			}
			else
			{
				for(int i =0; i < Stacks0.size(); i++)
				{
					Stack_lk0->lock();
					int val= Stacks0[i]->pop();
					StackOps0++;
					Stack_lk0->unlock();
				}
			}
		}
	}
	else {
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < Stacks1.size(); i++)
				{
					Stack_lk1->lock();
					Stacks1[i]->push(1);
					StackOps1++;
					Stack_lk1->unlock();
				}
			}
			else
			{
				for(int i =0; i < Stacks1.size(); i++)
				{
					Stack_lk1->lock();
					int val= Stacks1[i]->pop();
					StackOps1++;
					Stack_lk1->unlock();
				}
			}
		}
	}

	pthread_barrier_wait(&StackBar);
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
		std::cout << "Final StackOps: " << StackOps0+StackOps1 << std::endl;
	}	
	pthread_barrier_wait(&StackBar);
}

void* QueueTest(int tid, int duration, int node)
{
	pthread_barrier_wait(&QueueBar);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 1);

	if(node == 0){
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < Queues0.size(); i++)
				{
					Queue_lk0->lock();
					Queues0[i]->add(1);
					QueueOps0++;
					Queue_lk0->unlock();
				}
			}
			else
			{
				for(int i =0; i < Queues0.size(); i++)
				{
					Queue_lk0->lock();
					int val = Queues0[i]->del();
					QueueOps0++;
					Queue_lk0->unlock();
				}
			}
		}
	}
	else {
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < Queues1.size(); i++)
				{
					Queue_lk1->lock();
					Queues1[i]->add(1);
					QueueOps1++;
					Queue_lk1->unlock();
				}
			}
			else
			{
				for(int i =0; i < Queues1.size(); i++)
				{
					Queue_lk1->lock();
					int val = Queues1[i]->del();
					QueueOps1++;
					Queue_lk1->unlock();
				}
			}
		}
	}

	pthread_barrier_wait(&QueueBar);
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
		std::cout << "Final QueueOps: " << QueueOps0+QueueOps1 << std::endl;
	}
	pthread_barrier_wait(&QueueBar);
}



void* BinarySearchTest(int tid, int duration, int node)
{
	pthread_barrier_wait(&BSTBar);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 1);

	if(node == 0){
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < BSTs0.size(); i++)
				{
					BST_lk0->lock();
					BSTs0[i]->insert(1);
					BSTOps0++;
					BST_lk0->unlock();
				}
			}
			else
			{
				for(int i =0; i < BSTs0.size(); i++)
				{
					BST_lk0->lock();
					bool val = BSTs0[i]->lookup(1);
					BSTOps0++;
					BST_lk0->unlock();
				}
			}
		}
	}
	else {
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < BSTs1.size(); i++)
				{
					BST_lk1->lock();
					BSTs1[i]->insert(1);
					BSTOps1++;
					BST_lk1->unlock();
				}
			}
			else
			{
				for(int i =0; i < BSTs1.size(); i++)
				{
					BST_lk1->lock();
					bool val = BSTs1[i]->lookup(1);
					BSTOps1++;
					BST_lk1->unlock();
				}
			}
			
		}
	}

	pthread_barrier_wait(&BSTBar);
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
		std::cout << "Final BSTOps: " << BSTOps0+BSTOps1 << std::endl;
	}
	pthread_barrier_wait(&BSTBar);
}

void* LinkedListTest(int tid, int duration, int node)
{
	pthread_barrier_wait(&LLBar);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 1);

	if(node == 0){
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < LLs0.size(); i++)
				{
					LL_lk0->lock();
					LLs0[i]->append(1);
					LLOps0++;
					LL_lk0->unlock();
				}
			}
			else if(dist(gen) == 1)
			{
				for(int i =0; i < LLs0.size(); i++)
				{
					LL_lk0->lock();
					int val = LLs0[i]->removeHead();
					LLOps0++;
					LL_lk0->unlock();
				}
			}
			else
			{
				for(int i =0; i < LLs0.size(); i++)
				{
					LL_lk0->lock();
					bool val = LLs0[i]->lookUp(1);
					LLOps0++;
					LL_lk0->unlock();
				}
			}
		}
	}
	else {
		auto startTimer = std::chrono::steady_clock::now();
		auto endTimer = startTimer + std::chrono::seconds(duration);
		while (std::chrono::steady_clock::now() < endTimer) {
			if(dist(gen) == 0)
			{
				for(int i =0; i < LLs1.size(); i++)
				{
					LL_lk1->lock();
					LLs1[i]->append(1);
					LLOps1++;
					LL_lk1->unlock();
				}
			}
			else if(dist(gen)==1)
			{
				for(int i =0; i < LLs1.size(); i++)
				{
					LL_lk1->lock();
					int val = LLs1[i]->removeHead();
					LLOps1++;
					LL_lk1->unlock();
				}
			}
			else {
				for(int i =0; i < LLs1.size(); i++)
				{
					LL_lk1->lock();
					bool val = LLs1[i]->lookUp(1);
					LLOps1++;
					LL_lk1->unlock();
				}
			}
		}
	}
	pthread_barrier_wait(&LLBar);
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
		std::cout << "Final LLOps: " << LLOps0+LLOps1 << std::endl;
	}
	pthread_barrier_wait(&LLBar);
}







void global_cleanup(){
	delete Stack_lk0;
	delete Stack_lk1;
	pthread_barrier_destroy(&StackBar);

	delete Queue_lk0;
	delete Queue_lk1;
	pthread_barrier_destroy(&QueueBar);

	delete BST_lk0;
	delete BST_lk1;
	pthread_barrier_destroy(&BSTBar);

	delete LL_lk0;
	delete LL_lk1;
	pthread_barrier_destroy(&LLBar);
}