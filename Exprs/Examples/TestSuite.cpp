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
std::vector<mutex*> Stack_lk0;
std::vector<mutex*> Stack_lk1;
pthread_barrier_t StackBar;
std::mutex* printLK;

std::vector<numa<Queue,0>*> Queues0;
std::vector<numa<Queue,1>*> Queues1;
int64_t QueueOps0;
int64_t QueueOps1;
std::vector<mutex*> Queue_lk0;
std::vector<mutex*> Queue_lk1;
pthread_barrier_t QueueBar;


std::vector<numa<BinarySearchTree,0>*> BSTs0;
std::vector<numa<BinarySearchTree,1>*> BSTs1;
int64_t BSTOps0;
int64_t BSTOps1;
std::vector<mutex*> BST_lk0;
std::vector<mutex*> BST_lk1;
pthread_barrier_t BSTBar;

std::vector<numa<LinkedList,0>*> LLs0;
std::vector<numa<LinkedList,1>*> LLs1;
int64_t LLOps0;
int64_t LLOps1;
std::vector<mutex*> LL_lk0;
std::vector<mutex*> LL_lk1;
pthread_barrier_t LLBar;



// chrono::high_resolution_clock::time_point startTimer;
// chrono::high_resolution_clock::time_point endTimer;




void numa_Stack_init(int num_DS, int num_threads){
	Stacks0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Stacks0[i] = new numa<Stack,0>();
	}
	
	Stacks1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Stacks1[i] = new numa<Stack,1>();
	}

	Stack_lk0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Stack_lk0[i] = new mutex();
	}
	Stack_lk1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Stack_lk1[i] = new mutex();
	}
	pthread_barrier_init(&StackBar, NULL, num_threads);
	StackOps0 = 0;
	StackOps1 = 0;
	
	printLK = new std::mutex();
}

void numa_Queue_init(int num_DS, int num_threads){
	Queues0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Queues0[i] = new numa<Queue,0>();
	}
	
	Queues1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Queues1[i] = new numa<Queue,1>();
	}

	Queue_lk0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Queue_lk0[i] = new mutex();
	}
	Queue_lk1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		Queue_lk1[i] = new mutex();
	}
	pthread_barrier_init(&QueueBar, NULL, num_threads);
	QueueOps0 = 0;
	QueueOps1 = 0;
}

void numa_BST_init(int num_DS, int num_threads){
	BSTs0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		BSTs0[i] = new numa<BinarySearchTree,0>();
	}
	
	BSTs1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		BSTs1[i] = new numa<BinarySearchTree,1>();
	}

	BST_lk0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		BST_lk0[i] = new mutex();
	}
	BST_lk1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		BST_lk1[i] = new mutex();
	}
	pthread_barrier_init(&BSTBar, NULL, num_threads);
	BSTOps0 = 0;
	BSTOps1 = 0;
}

void numa_LL_init(int num_DS, int num_threads){
	LLs0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		LLs0[i] = new numa<LinkedList,0>();
	}
	
	LLs1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		LLs1[i] = new numa<LinkedList,1>();
	}

	LL_lk0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		LL_lk0[i] = new mutex();
	}
	LL_lk1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		LL_lk1[i] = new mutex();
	}
	pthread_barrier_init(&LLBar, NULL, num_threads);
	LLOps0 = 0;
	LLOps1 = 0;
}


void* StackTest(int tid, int duration, int node, int64_t num_DS, int num_threads)
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
				for(int stackIndex = tid; stackIndex < Stacks0.size(); stackIndex+=num_threads){
					Stack_lk0[stackIndex]->lock();
					Stacks0[stackIndex]->push(1);
					StackOps0++;
					Stack_lk0[stackIndex]->unlock();
				}
			}
			else
			{
				for(int stackIndex = tid; stackIndex < Stacks0.size(); stackIndex+=num_threads){
					Stack_lk0[stackIndex]->lock();
					int val= Stacks0[stackIndex]->pop();
					StackOps0++;
					Stack_lk0[stackIndex]->unlock();
	
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
				for(int stackIndex = tid; stackIndex < Stacks1.size(); stackIndex+=num_threads){
					Stack_lk1[stackIndex]->lock();
					Stacks1[stackIndex]->push(1);
					StackOps1++;
					Stack_lk1[stackIndex]->unlock();
				}
			}
			else
			{
				for(int stackIndex = tid; stackIndex < Stacks1.size(); stackIndex+=num_threads){
					Stack_lk1[stackIndex]->lock();
					int val= Stacks1[stackIndex]->pop();
					StackOps1++;
					Stack_lk1[stackIndex]->unlock();
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

void* QueueTest(int tid, int duration, int node, int64_t num_DS, int num_threads)
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
				for(int QueueIndex = tid; QueueIndex < Queues0.size(); QueueIndex+=num_threads){
					Queue_lk0[QueueIndex]->lock();
					Queues0[QueueIndex]->add(1);
					QueueOps0++;
					Queue_lk0[QueueIndex]->unlock();
				}
			}
			else
			{
				for(int QueueIndex = tid; QueueIndex < Queues0.size(); QueueIndex+=num_threads){
					Queue_lk0[QueueIndex]->lock();
					int val = Queues0[QueueIndex]->del();
					QueueOps0++;
					Queue_lk0[QueueIndex]->unlock();
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
				for(int QueueIndex = tid; QueueIndex < Queues1.size(); QueueIndex+=num_threads){
					Queue_lk1[QueueIndex]->lock();
					Queues1[QueueIndex]->add(1);
					QueueOps1++;
					Queue_lk1[QueueIndex]->unlock();
				}
			}
			else
			{
				for(int QueueIndex = tid; QueueIndex < Queues1.size(); QueueIndex+=num_threads){
					Queue_lk1[QueueIndex]->lock();
					int val = Queues1[QueueIndex]->del();
					QueueOps1++;
					Queue_lk1[QueueIndex]->unlock();
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
}



void* BinarySearchTest(int tid, int duration, int node, int64_t num_DS, int num_threads)
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
				for(int BSTIndex = tid; BSTIndex < BSTs0.size(); BSTIndex+=num_threads){
					BST_lk0[BSTIndex]->lock();
					BSTs0[BSTIndex]->insert(1);
					BSTOps0++;
					BST_lk0[BSTIndex]->unlock();
				}
			}
			else
			{
				for(int BSTIndex = tid; BSTIndex < BSTs0.size(); BSTIndex+=num_threads){
					BST_lk0[BSTIndex]->lock();
					bool val = BSTs0[BSTIndex]->lookup(1);
					BSTOps0++;
					BST_lk0[BSTIndex]->unlock();
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
				for(int BSTIndex = tid; BSTIndex < BSTs1.size(); BSTIndex+=num_threads){
					BST_lk1[BSTIndex]->lock();
					BSTs1[BSTIndex]->insert(1);
					BSTOps1++;
					BST_lk1[BSTIndex]->unlock();
				}
			}
			else
			{
				for(int BSTIndex = tid; BSTIndex < BSTs1.size(); BSTIndex+=num_threads){
					BST_lk1[BSTIndex]->lock();
					bool val = BSTs1[BSTIndex]->lookup(1);
					BSTOps1++;
					BST_lk1[BSTIndex]->unlock();
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

void* LinkedListTest(int tid, int duration, int node, int64_t num_DS, int num_threads)
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
				for(int LLIndex = tid; LLIndex < LLs0.size(); LLIndex+=2){
					LL_lk0[LLIndex]->lock();
					LLs0[LLIndex]->append(1);
					LLOps0++;
					LL_lk0[LLIndex]->unlock();
				}
			}
			else if(dist(gen) == 1)
			{
				for(int LLIndex = tid; LLIndex < LLs0.size(); LLIndex+=num_threads){
					LL_lk0[LLIndex]->lock();
					int val = LLs0[LLIndex]->removeHead();
					LLOps0++;
					LL_lk0[LLIndex]->unlock();
				}
			}
			else
			{
				for(int LLIndex = tid; LLIndex < LLs0.size(); LLIndex+=num_threads){
					LL_lk0[LLIndex]->lock();
					bool val = LLs0[LLIndex]->lookUp(1);
					LLOps0++;
					LL_lk0[LLIndex]->unlock();
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
				for(int LLIndex = tid; LLIndex < LLs1.size(); LLIndex+=num_threads){
					LL_lk1[LLIndex]->lock();
					LLs1[LLIndex]->append(1);
					LLOps1++;
					LL_lk1[LLIndex]->unlock();
				}
			}
			else if(dist(gen)==1)
			{
				for(int LLIndex = tid; LLIndex < LLs1.size(); LLIndex+=num_threads){
					LL_lk1[LLIndex]->lock();
					int val = LLs1[LLIndex]->removeHead();
					LLOps1++;
					LL_lk1[LLIndex]->unlock();
				}
			}
			else {
				for(int LLIndex = tid; LLIndex < LLs1.size(); LLIndex+=num_threads){
					LL_lk1[LLIndex]->lock();
					bool val = LLs1[LLIndex]->lookUp(1);
					LLOps1++;
					LL_lk1[LLIndex]->unlock();
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
	for(int i = 0; i < Stacks0.size(); i++)
	{
		delete Stacks0[i];
	}
	for(int i = 0; i < Stacks1.size(); i++)
	{
		delete Stacks1[i];
	}

	pthread_barrier_destroy(&StackBar);

	for(int i = 0; i < Queues0.size(); i++)
	{
		delete Queues0[i];
	}
	for(int i = 0; i < Queues1.size(); i++)
	{
		delete Queues1[i];
	}
	pthread_barrier_destroy(&QueueBar);

	for(int i = 0; i < BSTs0.size(); i++)
	{
		delete BSTs0[i];
	}
	for(int i = 0; i < BSTs1.size(); i++)
	{
		delete BSTs1[i];
	}
	pthread_barrier_destroy(&BSTBar);

	for(int i = 0; i < LLs0.size(); i++)
	{
		delete LLs0[i];
	}
	pthread_barrier_destroy(&LLBar);
}