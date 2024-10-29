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

int64_t num_DS;
int num_threads;
int duration;
string DS_name;



int main(int argc, char *argv[])
{

	    // Define long options
    static struct option long_options[] = {
        {"threads", required_argument, 0, 't'},
        {"duration", required_argument, 0, 'd'},
        {"num_DS", required_argument, 0, 'n'},  // Long option without short equivalent
		{"DS_name", required_argument, 0, 0},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "t:d:n:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 't':
                num_threads = std::stoi(optarg);
				// if(num_threads % 2 != 0){cout<<"Number of threads must be even"<<endl; return 1;}
                break;
            case 'd':
                duration = std::stoi(optarg);
                break;
            case 'n':
                num_DS = std::stoi(optarg);    
				if(num_DS % 2 != 0){cout<<"Number of Data Structures must be even"<<endl; return 1;}
                break;
			case 0:
				if (std::string(long_options[option_index].name) == "DS_name") {
					DS_name = std::string(optarg);
				}
				break;
            default:
                std::cerr << "Usage: " << argv[0] << " -t <num_threads> -d <duration> -n <number of Data Structures> --DS= <data-structure>\n";
                return 1;
        }
    }

	cout<<"Number of threads: "<<num_threads<<endl;
	cout<<"Duration: "<<duration<<endl;
	cout<<"Number of Data Structures: "<<num_DS<<endl;
	cout<<"DS name: "<<DS_name<<endl;

	std::vector <thread_numa<0>*> thread0;
	std::vector <thread_numa<1>*> thread1;

	thread0.resize(num_threads);
	thread1.resize(num_threads);

	if(DS_name == "stack"){
		cout<<"Testing Stack"<<endl;
		numa_Stack_init(num_DS/2, num_threads);
		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			std::cout<< "Thread" << tid << " spawned" << std::endl;
			thread0[i] = new thread_numa<0>(StackTest,tid, duration, node, num_DS/2, num_threads/2);
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;
			std::cout<< "Thread" << tid << " spawned" << std::endl;
			thread1[i] = new thread_numa<1>(StackTest,tid, duration, node, num_DS/2, num_threads/2);
		}

		for(int i=0; i < thread0.size(); i++){
			thread0[i]->join();
		}

		for(int i=0; i < thread1.size(); i++){
			thread1[i]->join();
		}
	}

	else if(DS_name == "queue"){
		cout<<"Testing Queue"<<endl;
		numa_Queue_init(num_DS/2,num_threads);
		cout<<"about to spawn threads"<<endl;
		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			thread0[i] = new thread_numa<0>(QueueTest,tid, duration, node, num_DS/2, num_threads/2);
			std::cout<< "Thread" << i << " spawned" << std::endl;
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;
			thread1[i] = new thread_numa<1>(QueueTest,tid , duration, node, num_DS/2, num_threads/2);
			
		}

		for(int i=0; i < thread0.size(); i++){
			cout<<"Joining thread "<<i<<endl;
			thread0[i]->join();
		}
		for(int i=0; i < thread1.size()/2; i++){
			thread1[i]->join();
		}
	}

	else if(DS_name == "bst"){
		cout<<"Testing Binary Search Tree"<<endl;
		numa_BST_init(num_DS/2,num_threads);
		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			thread0[i] = new thread_numa<0>(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;
			thread1[i] = new thread_numa<1>(BinarySearchTest,tid, duration, node, num_DS/2, num_threads/2);
		}

		for(int i=0; i < thread0.size(); i++){
			thread0[i]->join();
		}
		for(int i=0; i < thread1.size(); i++){
			thread1[i]->join();
		}
	}

	else if(DS_name == "ll"){
		cout<<"Testing Linked List"<<endl;
		numa_LL_init(num_DS/2,num_threads);
		for(int i=0; i < num_threads/2; i++){
			int node = 0;
			int tid = i;
			thread0[i] = new thread_numa<0>(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
		}
		for(int i=0; i < num_threads/2; i++){
			int node = 1;
			int tid = i + num_threads/2;
			thread1[i] = new thread_numa<1>(LinkedListTest,tid, duration, node, num_DS/2, num_threads);
		}

		for(int i=0; i < thread0.size(); i++){
			thread0[i]->join();
		}
		for(int i=0; i < thread1.size(); i++){
			thread1[i]->join();
		}
	}

	else{
		cout<<"Invalid Data Structure"<<endl;
	}
	global_cleanup();
}