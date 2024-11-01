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


std::vector<Stack*> Stacks0;
std::vector<Stack*> Stacks1;
int64_t ops0=0;
int64_t ops1=0;
std::vector<mutex*> Stack_lk0;
std::vector<mutex*> Stack_lk1;
pthread_barrier_t bar ;

std::mutex* printLK;
std::mutex* globalLK;



std::vector<Queue*> Queues0;
std::vector<Queue*> Queues1;
std::vector<mutex*> Queue_lk0;
std::vector<mutex*> Queue_lk1;


std::vector<BinarySearchTree*> BSTs0;
std::vector<BinarySearchTree*> BSTs1;
std::vector<mutex*> BST_lk0;
std::vector<mutex*> BST_lk1;

std::vector<LinkedList*> LLs0;
std::vector<LinkedList*> LLs1;
std::vector<mutex*> LL_lk0;
std::vector<mutex*> LL_lk1;



// chrono::high_resolution_clock::time_point startTimer;
// chrono::high_resolution_clock::time_point endTimer;




void numa_Stack_init(std::string DS_config, int num_DS, int num_threads){
	Stacks0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			//cout<<"Initializing node 0 numa stack pool"<<endl;
			Stacks0[i] = reinterpret_cast<Stack*>(new numa<Stack,0>());
		}
		else{
			//cout<<"Initializing first regular stack pool"<<endl;
			Stacks0[i] = new Stack();
		}
	}
	
	Stacks1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			//cout<<"Initializing node 1 numa stack pool"<<endl;
			Stacks1[i] = reinterpret_cast<Stack*>(new numa<Stack,1>());
		}
		else{
			//cout<<"Initializing second regular stack pool"<<endl;
			Stacks1[i] = new Stack();
		}
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
	pthread_barrier_init(&bar, NULL, num_threads);
	ops0 = 0;
	ops1 = 0;
	
	printLK = new std::mutex();
	globalLK = new std::mutex();
}

void numa_Queue_init(std::string DS_config, int num_DS, int num_threads){
	Queues0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			Queues0[i] = reinterpret_cast<Queue*>(new numa<Queue,0>());
		}
		else{
			Queues0[i] = new Queue();
		}
	}
	
	Queues1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			Queues1[i] = reinterpret_cast<Queue*>(new numa<Queue,1>());
		}
		else{
			Queues1[i] = new Queue();
		}
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
	pthread_barrier_init(&bar, NULL, num_threads);
	ops0 = 0;
	ops1 = 0;
	
	printLK = new std::mutex();
	globalLK = new std::mutex();
	std::cout<<"Queue initialized"<<std::endl;
}

void numa_LL_init(std::string DS_config, int num_DS, int num_threads){
	LLs0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			LLs0[i] = reinterpret_cast<LinkedList*>(new numa<LinkedList,0>());
		}
		else{
			LLs0[i] = new LinkedList();
		}
	}
	
	LLs1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			LLs1[i] = reinterpret_cast<LinkedList*>(new numa<LinkedList,1>());
		}
		else{
			LLs1[i] = new LinkedList();
		}
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
	pthread_barrier_init(&bar, NULL, num_threads);
	ops0 = 0;
	ops1 = 0;
	
	printLK = new std::mutex();
	globalLK = new std::mutex();
}


void numa_BST_init(std::string DS_config, int num_DS, int num_threads){
	BSTs0.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			BSTs0[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree,0>());
		}
		else{
			BSTs0[i] = new BinarySearchTree();
		}
	}
	
	BSTs1.resize(num_DS);
	for(int i = 0; i < num_DS; i++)
	{
		if(DS_config=="numa"){
			BSTs1[i] = reinterpret_cast<BinarySearchTree*>(new numa<BinarySearchTree,1>());
		}
		else{
			BSTs1[i] = new BinarySearchTree();
		}
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

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist1(0, BSTs0.size()-1);
	std::uniform_int_distribution<> dist2(0, BSTs1.size()-1);

	//Prefill in 50 % of the tree randomly with value 1
	for(int i = 0; i < num_DS/2; i++)
	{
		int ds = dist1(gen);
		BST_lk0[ds]->lock();
		BSTs0[ds]->insert(dist1(gen));
		BST_lk0[ds]->unlock();
	}

	for(int i = 0; i < num_DS/2; i++)
	{
		int ds = dist2(gen);
		BST_lk1[ds]->lock();
		BSTs1[ds]->insert(dist2(gen));
		BST_lk1[ds]->unlock();
	}
	

	pthread_barrier_init(&bar, NULL, num_threads);
	ops0 = 0;
	ops1 = 0;
	
	printLK = new std::mutex();
	globalLK = new std::mutex();
}

void StackTest(int tid,  int duration, int node, int64_t num_DS, int num_threads)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;

	}		
	#endif

	pthread_barrier_wait(&bar);
    std::mt19937 gen(123);
    std::uniform_int_distribution<> dist(0, Stacks0.size()-1);
	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int op = dist(gen)%2;
		
		if(node==0){
			if(op == 0)
			{
				Stack_lk0[ds]->lock();
				Stacks0[ds]->push(1);
				Stack_lk0[ds]->unlock();
			}
			else
			{
				Stack_lk0[ds]->lock();
				int val= Stacks0[ds]->pop();
				Stack_lk0[ds]->unlock();
			}
		}
		else{
			if(op == 0)
			{
				Stack_lk1[ds]->lock();
				Stacks1[ds]->push(1);
				Stack_lk1[ds]->unlock();
			}
			else
			{
				Stack_lk1[ds]->lock();
				int val= Stacks1[ds]->pop();
				Stack_lk1[ds]->unlock();
			}
		}
		ops++;
	}


	globalLK->lock();
	if(node==0)
	{
		ops0 += ops;
	}
	else
	{
		ops1 += ops;
	}
	globalLK->unlock();

	pthread_barrier_wait(&bar);
}

void QueueTest(int tid, int duration, int node, int64_t num_DS, int num_threads)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif
	globalLK->lock();
	std::cout<<"Random number bounded"<<endl;
	globalLK->unlock();

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, Queues0.size()-1);

	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int op = dist(gen)%2;
		
		if(node==0){
			if(op == 0)
			{
				Queue_lk0[ds]->lock();
				Queues0[ds]->add(ds);
				Queue_lk0[ds]->unlock();
			}
			else
			{
				Queue_lk0[ds]->lock();
				int val= Queues0[ds]->del();
				Queue_lk0[ds]->unlock();
			}
		}
		else{
			if(op == 0)
			{
				Queue_lk1[ds]->lock();
				Queues1[ds]->add(ds);
				Queue_lk1[ds]->unlock();
			}
			else
			{
				Queue_lk1[ds]->lock();
				int val= Queues1[ds]->del();
				Queue_lk1[ds]->unlock();
			}
		}
		ops++;
	}

	globalLK->lock();
	if(node==0)
	{
		ops0 += ops;
	}
	else
	{
		ops1 += ops;
	}
	globalLK->unlock();

	pthread_barrier_wait(&bar);
}


void LinkedListTest(int tid, int duration, int node, int64_t num_DS, int num_threads)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	pthread_barrier_wait(&bar);
	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, LLs0.size()-1);
	std::uniform_int_distribution<> opDist(1, 100);
	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int op = dist(gen)%2;
		if(node==0){
			if(op == 0)
			{
				LL_lk0[ds]->lock();
				LLs0[ds]->append(ds);
				LL_lk0[ds]->unlock();
			}
			else
			{
				LL_lk0[ds]->lock();
				LLs0[ds]->removeHead();
				LL_lk0[ds]->unlock();
			}
		}
		else{
			if(op == 0)
			{
				LL_lk1[ds]->lock();
				LLs1[ds]->append(ds);
				LL_lk1[ds]->unlock();
			}
			else
			{
				LL_lk1[ds]->lock();
				LLs1[ds]->removeHead();
				LL_lk1[ds]->unlock();
			}
		}
		ops++;
	}

	globalLK->lock();
	if(node==0)
	{
		ops0 += ops;
	}
	else
	{
		ops1 += ops;
	}

	globalLK->unlock();

	pthread_barrier_wait(&bar);
}

void BinarySearchTest(int tid, int duration, int node, int64_t num_DS, int num_threads)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	pthread_barrier_wait(&bar);
	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, BSTs0.size()-1);
	std::uniform_int_distribution<> opDist(1, 100);
	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		if(node==0){
			if(opDist(gen)<=40)
			{
				BST_lk0[ds]->lock();
				BSTs0[ds]->lookup(1);
				BST_lk0[ds]->unlock();
			}
			else if(opDist(gen)<=80){
				BST_lk0[ds]->lock();
				BSTs0[ds]->update(ds);
				BST_lk0[ds]->unlock();
			}
			else if(opDist(gen)<=90){
				BST_lk0[ds]->lock();
				BSTs0[ds]->insert(ds);
				BST_lk0[ds]->unlock();
			}
			else{
				BST_lk0[ds]->lock();
				BSTs0[ds]->remove(ds);
				BST_lk0[ds]->unlock();
			}
		}
		else{
			if(opDist(gen)<=40)
			{
				BST_lk1[ds]->lock();
				BSTs1[ds]->lookup(ds);
				BST_lk1[ds]->unlock();
			}
			else if(opDist(gen)<=80){
				BST_lk1[ds]->lock();
				BSTs1[ds]->update(ds);
				BST_lk1[ds]->unlock();
			}
			else if(opDist(gen)<=90){
				BST_lk1[ds]->lock();
				BSTs1[ds]->insert(ds);
				BST_lk1[ds]->unlock();
			}
			else{
				BST_lk1[ds]->lock();
				BSTs1[ds]->remove(ds);
				BST_lk1[ds]->unlock();
			}
		}
		ops++;
	}

	globalLK->lock();
	if(node==0)
	{
		ops0 += ops;
	}
	else
	{
		ops1 += ops;
	}
	globalLK->unlock();

	pthread_barrier_wait(&bar);

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

	for(int i = 0; i < Queues0.size(); i++)
	{
		delete Queues0[i];
	}
	for(int i = 0; i < Queues1.size(); i++)
	{
		delete Queues1[i];
	}

	for(int i = 0; i < BSTs0.size(); i++)
	{
		delete BSTs0[i];
	}
	for(int i = 0; i < BSTs1.size(); i++)
	{
		delete BSTs1[i];
	}

	for(int i = 0; i < LLs0.size(); i++)
	{
		delete LLs0[i];
	}
}