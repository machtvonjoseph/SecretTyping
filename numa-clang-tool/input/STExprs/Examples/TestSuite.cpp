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
// #include "numatype.hpp"
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
#include "umf_numa_allocator.hpp"

#define MEGABYTE 1048576

std::vector<Stack*> Stacks0;
std::vector<Stack*> Stacks1;
int64_t ops0=0;
int64_t ops1=0;

char* Arrays0;
char* Arrays1;

std::vector<mutex*> Stack_lk0;
std::vector<mutex*> Stack_lk1;
pthread_barrier_t bar ;
pthread_barrier_t init_bar;

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
std::vector<mutex*> BST_reader_lk0;
std::vector<mutex*> BST_reader_lk1;

std::vector<LinkedList*> LLs0;
std::vector<LinkedList*> LLs1;
std::vector<mutex*> LL_lk0;
std::vector<mutex*> LL_lk1;

mutex* Array_Lk0;
mutex* Array_Lk1;

struct prefill_percentage{
	float write;
	float read;
	float remove;
	float update;
};

// chrono::high_resolution_clock::time_point startTimer;
// chrono::high_resolution_clock::time_point endTimer;

void global_init(int num_threads){
	ops0 = 0;
}

void singleThreadedStackInit(int num_DS, bool isNuma){
	Stacks0.resize(num_DS);
	if(isNuma){
		cout<<"Initializing numa stack pool"<<endl;
		for(int i = 0; i < num_DS; i++)
		{
			Stacks0[i] = reinterpret_cast<Stack*>(new numa<Stack,0>());
		}
	}
	else{
		cout<<"Initializing regular stack pool"<<endl;
		for(int i = 0; i < num_DS; i++)
		{
			Stacks0[i] = new Stack();
		}
	}
}


void numa_array_init(std::string DS_config, int size, bool prefill, prefill_percentage &percentages){
	if(DS_config=="numa"){	
		Arrays0 = (char*)umf_alloc(0, sizeof(char)* size, alignof(char));
	}
	else{
		Arrays0 = new char[size];
	}
	if(DS_config=="numa"){	
		Arrays1 = (char*)umf_alloc(1, sizeof(char)* size, alignof(char));
	}
	else{
		Arrays1 = new char[size];
	}
	if(prefill){
		for(int i = 0; i < size/int(percentages.write); i++){
			Arrays0[i] = 'a';
			Arrays1[i] = 'a';
		}
	}
}

void numa_Stack_init(std::string DS_config, int num_DS, bool prefill, prefill_percentage &percentages){
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

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist1(0, Stacks0.size()-1);
	//Prefill in 50 % of the Stacks with random number of nodes (0-200) number of nodes
	for(int i = 0; i < num_DS/2 ; i++)
	{
		int ds = dist1(gen);
		for(int j=0; j < 200*1024; j++)
		{
			Stacks0[ds]->push(ds);
		}
	}
}

void numa_Queue_init(std::string DS_config, int num_DS, bool prefill, prefill_percentage &percentages){
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

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist1(0, Queues0.size()-1);
	//Prefill in 50 % of the Stacks with random number of nodes (0-200) number of nodes
	for(int i = 0; i < num_DS/2 ; i++)
	{
		int ds = dist1(gen);
		for(int j=0; j < 200*1024; j++)
		{
			Queues0[ds]->add(ds);
		}
	}
}

void numa_LL_init(std::string DS_config, int num_DS, bool prefill, prefill_percentage &percentages){
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

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist1(0, LLs0.size()-1);
	// std::uniform_int_distribution<> dist(0, keyspace/2);
	//Prefill in 50 % of the Stacks with random number of nodes (0-200) number of nodes
	for(int i = 0; i < num_DS/2 ; i++)
	{
		int ds = dist1(gen);
		for(int j=0; j < 200*1024; j++)
		{
			LLs0[ds]->append(ds);
		}
	}
	std::cout<<"LL is initialized"<<std::endl;
}


void numa_BST_init(std::string DS_config, int num_DS, int keyspace, int crossover){
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

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, keyspace/2);
	
	for(int i = 0; i < num_DS/2 ; i++)
	{
		for(int j=0; j < keyspace/2; j++)
		{
			BSTs0[i]->insert(dist(gen));
		}
	}	

}

void singleThreadedStackTest(int duration, int64_t num_DS){
	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, Stacks0.size()-1);
	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int op = dist(gen)%2;
		
			if(op == 0){
				Stacks0[ds]->push(1);
				ops++;
			}
			else{
				int val= Stacks0[ds]->pop();
				ops++;
			}
	}

	std::cout << "OPS FOR SINGLE THREAD IS: " << ops << std::endl;
}



void ArrayTest(int tid,  int duration, int node, int64_t num_DS, int num_threads, int crossover){

	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int x = 0;
		if(node==0){
			Array_Lk0->lock();
			Arrays0[x] = 1;
			Array_Lk0->unlock();
			
		}
		else{
			Array_Lk1->lock();
			Arrays1[x] = 1;
			Array_Lk1->unlock();
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


void StackTest(int tid,  int duration, int node, int64_t num_DS, int num_threads, int crossover)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;

	}		
	#endif

    std::mt19937 gen(123);
    std::uniform_int_distribution<> dist(0, Stacks0.size()-1);
	std::uniform_int_distribution<> opDist(1, 100);
	std::uniform_int_distribution<> xDist(1, 100);
	
	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int op = dist(gen)%2;
		int x = xDist(gen);
		if(node==0){
			if(opDist(gen)<=50)
			{
				Stacks0[ds]->push(ds);
			}
			else
			{
				int val= Stacks0[ds]->pop();
			}
		}
		ops++;
	}
	ops0 = ops;
}

void QueueTest(int tid, int duration, int node, int64_t num_DS, int num_threads, int crossover)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}
	#endif

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, Queues0.size()-1);
	std::uniform_int_distribution<> opDist(1, 100);
	std::uniform_int_distribution<> xDist(1, 100);

	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int x = xDist(gen);
		if(node==0){
			if(opDist(gen)<=50)
			{
				Queues0[ds]->add(ds);
			}
			else {
				int value = Queues0[ds]->del();
			}
		}
		ops++;
	}
	ops0 = ops;
}


void LinkedListTest(int tid, int duration, int node, int64_t num_DS, int num_threads, int crossover)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, LLs0.size()-1);
	std::uniform_int_distribution<> opDist(1, 100);
	std::uniform_int_distribution<> xDist(1, 100);
	//std::cout << "Thread " << tid << " about to start working on node id"<<node << std::endl;
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int x = xDist(gen);
		if(node==0){
			if(opDist(gen)<=90)
			{
				LLs0[ds]->append(ds);
			}
			else {
				LLs0[ds]->removeHead();
			}
		}
		ops++;
	}
	ops0 = ops;
}

void BinarySearchTest(int tid, int duration, int node, int64_t num_DS, int num_threads, int crossover, int keyspace)
{	
	#ifdef DEBUG
	if(tid == 1 && node==0)
	{	// startTime = chrono::high_resolution_clock::now();
		std::cout << "Only thread "<< tid << " will print this." << std::endl;
	}		
	#endif

	std::mt19937 gen(123);
	std::uniform_int_distribution<> dist(0, BSTs0.size()-1);
	std::uniform_int_distribution<> opDist(1, 100);
	std::uniform_int_distribution<> xDist(1, 100);
	std::uniform_int_distribution<> keyDist(0,keyspace);
	int ops = 0;
	auto startTimer = std::chrono::steady_clock::now();
	auto endTimer = startTimer + std::chrono::seconds(duration);
	while (std::chrono::steady_clock::now() < endTimer) {
		int ds = dist(gen);
		int key = keyDist(gen);
		if(node==0){
			if(opDist(gen)<=90)
			{
				BSTs0[ds]->insert(key);
			}
			else {
				bool value;
				value = BSTs0[ds]->lookup(key);
			}
		}
		ops++;
	}
	ops0 = ops;
}



void global_cleanup(){
}