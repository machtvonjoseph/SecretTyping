/*! \file main.cpp
 * \brief Test Suite for classic Data Structures.
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#include "TestSuite.hpp"
#include <thread>
#include <barrier>
#include <mutex>
#include <iostream>
#include <syncstream>
#include <string>
#include <vector>
#include "numatype.hpp"
#include "numathreads.hpp"
#include <getopt.h>


#define NUMA_NODES 4
using namespace std;


std::string thread_config;
std::string DS_config;
int64_t num_DS = 0;
int num_threads = 0;
int duration = 0;
std::string DS_name;


extern int64_t ops0;
extern int64_t ops1;



int main(int argc, char *argv[])
{

	    // Define long options
	static struct option long_options[] = {
		{"th_config", required_argument, nullptr, 'c'},     // --th_config=NUMA/REGULAR
		{"DS_config", required_argument, nullptr, 'd'},     // --DS_config=NUMA/REGULAR
		{"DS_name", required_argument, nullptr, 's'},       // --DS_name=STACK/QUEUE
		{"num_DS", required_argument, nullptr, 'n'},        // -n
		{"num_threads", required_argument, nullptr, 't'},   // -t
		{"duration", required_argument, nullptr, 'D'},      // -d
		{nullptr, 0, nullptr, 0}                            // End of array
	};

 int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "n:t:D:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c':  // --th_config option
                thread_config = optarg;             
                break;
            case 'd':  // --DS_config option
                DS_config = optarg;
                break;
            case 'n':  // -n option for num_DS
                num_DS = std::stoll(optarg);
                break;
            case 't':  // -t option for num_threads
                num_threads = std::stoi(optarg);
                break;
            case 'D':  // -D option for duration
                duration = std::stoi(optarg);
                break;
            case 's':  // --DS_name option
                DS_name = optarg;
                break;
            case '?':  // Unknown option
                std::cerr << "Unknown option or missing argument.\n";
                return 1;
            default:
                break;
        }
    }
    std::cout << "Final configuration:\n";
    std::cout << "Thread Config: " << thread_config << "\n";
    std::cout << "DS Config: " << DS_config << "\n";
    std::cout << "Number of DS: " << num_DS << "\n";
    std::cout << "Number of Threads: " << num_threads << "\n";
    std::cout << "Duration: " << duration << "\n";
    std::cout << "DS Name: " << DS_name << "\n";

	std::vector <thread_numa<0>*> numa_thread0;
	std::vector <thread_numa<1>*> numa_thread1;
	std::vector <thread*> regular_thread0;
	std::vector <thread*> regular_thread1;

	numa_thread0.resize(num_threads);
	numa_thread1.resize(num_threads);
	regular_thread0.resize(num_threads);
	regular_thread1.resize(num_threads);

	if(DS_name == "stack"){
		cout<<"Testing Stack"<<endl;
		numa_Stack_init(DS_config, num_DS/2, num_threads);

		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			if(thread_config == "numa"){
				std::cout<< "Thread" << tid << " spawned on numa node 0 threads" << std::endl;
				numa_thread0[i] = new thread_numa<0>(StackTest, tid, duration, node, num_DS/2, num_threads/2);
			}
			else if(thread_config== "regular"){
				std::cout<< "Thread" << tid << " spawned from regular thread pool 0" << std::endl;
				regular_thread0[i] = new thread(StackTest, tid, duration, node, num_DS/2, num_threads/2);
			}
			else{
				// if(i%2==0){
				// 	std::cout<< "Thread" << tid << " is even and is spawned on numa node 0 threads" << std::endl;
				// 	numa_thread0[i] = new thread_numa<0>(StackTest, tid, duration, node, num_DS/2, num_threads/2);
				// }
				// else{
				// 	std::cout<< "Thread" << tid << " is odd and is spawned on numa node 0 threads" << std::endl;
				// 	numa_thread0[i] = new thread(StackTest, tid, duration, node, num_DS/2, num_threads/2);
				// }
			}
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;

			if(thread_config == "numa"){
				std::cout<< "Thread" << tid << " spawned on numa node 1 threads" << std::endl;
				numa_thread1[i] = new thread_numa<1>(StackTest, tid, duration, node, num_DS/2, num_threads/2);
			}
			else if(thread_config== "regular"){
				std::cout<< "Thread" << tid << " spawned from regular thread pool 1" << std::endl;
				regular_thread1[i] = new thread(StackTest, tid, duration, node, num_DS/2, num_threads/2);
			}
			else{
				// if(i%2==0){
				// 	std::cout<< "Thread" << tid << " is even and is spawned on numa node 1 threads" << std::endl;
				// 	numa_thread1[i] = new thread_numa<1>(StackTest, tid, duration, node, num_DS/2, num_threads/2);
				// }
				// else{
				// 	std::cout<< "Thread" << tid << " is odd and is spawned from numa node 1 threads" << std::endl;
				// 	numa_thread1[i] = new thread_numa<1>(StackTest, tid, duration, node, num_DS/2, num_threads/2);
				// }
			}
		}

		if(thread_config == "numa"){
			for(int i=0; i < num_threads/2; i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < num_threads/2; i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}
		else if(thread_config == "regular"){
			for(int i=0; i < regular_thread0.size(); i++){
				if(regular_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread0[i]->join();
			}
			for(int i=0; i < regular_thread1.size(); i++){
				if(regular_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread1[i]->join();
			}
		}
		else{
			for(int i=0; i < numa_thread0.size(); i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < numa_thread1.size(); i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}

		std::cout << "Stack operations on pool 0: " << ops0 << std::endl;
		std::cout << "Stack operations on pool 1: " << ops1 << std::endl;
		std::cout << "Total operations: " << ops0 + ops1 << std::endl;
	}

	else if(DS_name == "queue"){
		cout<<"Testing Queue"<<endl;
		numa_Queue_init(DS_config,num_DS/2,num_threads);
		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			if(thread_config == "numa"){
				numa_thread0[i] = new thread_numa<0>(QueueTest,tid, duration, node, num_DS/2, num_threads/2);
			}
			else if(thread_config == "regular"){
				regular_thread0[i] = new thread(QueueTest,tid, duration, node, num_DS/2, num_threads/2);
			}
			else{
				// if(i%2==0){
				// 	numa_thread0[i] = new thread_numa<0>(QueueTest,tid, duration, node, num_DS/2, num_threads/2);
				// }
				// else{
				// 	numa_thread0[i] = new thread(QueueTest,tid, duration, node, num_DS/2, num_threads/2);
				// }
			}
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;
			if(thread_config == "numa"){
				numa_thread1[i] = new thread_numa<1>(QueueTest,tid , duration, node, num_DS/2, num_threads/2);
			}
			else if(thread_config == "regular"){
				regular_thread1[i] = new thread(QueueTest,tid , duration, node, num_DS/2, num_threads/2);
			}
			else{
				// if(i%2==0){
				// 	numa_thread1[i] = new thread_numa<1>(QueueTest,tid , duration, node, num_DS/2, num_threads/2);
				// }
				// else{
				// 	numa_thread1[i] = new thread(QueueTest,tid , duration, node, num_DS/2, num_threads/2);
				// }
			}
		}

		if(thread_config == "numa"){
			
			for(int i=0; i < numa_thread0.size(); i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < numa_thread1.size(); i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}

		else if(thread_config == "regular"){
			for(int i=0; i < regular_thread0.size(); i++){
				if(regular_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread0[i]->join();
			}
			for(int i=0; i < regular_thread1.size(); i++){
				if(regular_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread1[i]->join();
			}
		}
		else{
			for(int i=0; i < numa_thread0.size(); i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < numa_thread1.size(); i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}

		std::cout << "Queue operations on pool 0: " << ops0 << std::endl;
		std::cout << "Queue operations on pool 1: " << ops1 << std::endl;
		std::cout << "Total operations: " << ops0 + ops1 << std::endl;
	}



	else if(DS_name == "bst"){
		cout<<"Testing Binary Search Tree"<<endl;
		numa_BST_init(DS_config, num_DS/2, num_threads);
		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			if(thread_config == "numa"){
				numa_thread0[i] = new thread_numa<0>(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
			}
			else if(thread_config == "regular"){
				regular_thread0[i] = new thread(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
			}
			else{
				// if(i%2==0){
				// 	numa_thread0[i] = new thread_numa<0>(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
				// }
				// else{
				// 	numa_thread0[i] = new thread(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
				// }
			}
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;
			if(thread_config == "numa"){
				numa_thread1[i] = new thread_numa<1>(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
			}
			else if(thread_config == "regular"){
				regular_thread1[i] = new thread(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
			}
			else{
				// if(i%2==0){
				// 	numa_thread1[i] = new thread_numa<1>(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
				// }
				// else{
				// 	numa_thread1[i] = new thread(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
				// }
			}
		}

		if(thread_config == "numa"){
			for(int i=0; i < num_threads/2; i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < num_threads/2; i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}
		else if(thread_config == "regular"){
			for(int i=0; i < regular_thread0.size(); i++){
				if(regular_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread0[i]->join();
			}
			for(int i=0; i < regular_thread1.size(); i++){
				if(regular_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread1[i]->join();
			}
		}
		else{
			for(int i=0; i < numa_thread0.size(); i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < numa_thread1.size(); i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}

		std::cout << "Binary Search Tree operations on pool 0: " << ops0 << std::endl;
		std::cout << "Binary Search Tree operations on pool 1: " << ops1 << std::endl;
		std::cout << "Total operations: " << ops0 + ops1 << std::endl;
	}
	else if(DS_name == "ll"){
		cout<<"Testing Linked List"<<endl;
		numa_LL_init(DS_config, num_DS/2, num_threads);
		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			if(thread_config == "numa"){
				numa_thread0[i] = new thread_numa<0>(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
			}
			else if(thread_config == "regular"){
				regular_thread0[i] = new thread(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
			}
			else{
				// if(i%2==0){
				// 	numa_thread0[i] = new thread_numa<0>(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
				// }
				// else{
				// 	numa_thread0[i] = new thread(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
				// }
			}
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;
			if(thread_config == "numa"){
				numa_thread1[i] = new thread_numa<1>(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
			}
			else if(thread_config == "regular"){
				regular_thread1[i] = new thread(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
			}
			else{
				// if(i%2==0){
				// 	numa_thread1[i] = new thread_numa<1>(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
				// }
				// else{
				// 	numa_thread1[i] = new thread(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
				// }
			}
		}

		if(thread_config == "numa"){
			for(int i=0; i < numa_thread0.size(); i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < numa_thread1.size(); i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}


		else if(thread_config == "regular"){
			for(int i=0; i < regular_thread0.size(); i++){
				if(regular_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread0[i]->join();
			}
			for(int i=0; i < regular_thread1.size(); i++){
				if(regular_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				regular_thread1[i]->join();
			}
		}
		else{
			for(int i=0; i < numa_thread0.size(); i++){
				if(numa_thread0[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread0[i]->join();
			}
			for(int i=0; i < numa_thread1.size(); i++){
				if(numa_thread1[i] == nullptr){
					std::cout << "Thread " << i << " is null" << std::endl;
					continue;
				}
				numa_thread1[i]->join();
			}
		}

		std::cout << "Linked List operations on pool 0: " << ops0 << std::endl;
		std::cout << "Linked List operations on pool 1: " << ops1 << std::endl;
		std::cout << "Total operations: " << ops0 + ops1 << std::endl;
	}
	
	else{
		cout<<"Invalid Data Structure"<<endl;
	}
	global_cleanup();
}
