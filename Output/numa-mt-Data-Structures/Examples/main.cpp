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


int main(int argc, char *argv[])
{
	int num_threads = std::stoi(argv[1]);
	std::vector <thread_numa<0>*> thread0;

	
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "Welcome to the Multi Threaded Test Suite for Data Structures" << std::endl;

	std::cout << "Initializing Test Suite with " << num_threads << " threads." << std::endl;
	thread0.resize(num_threads);

	DS_init();
	sync_init(num_threads);
	
	for(int i=0; i < num_threads; i++){
		thread0[i] = new thread_numa<0>(StackTest,i, num_threads);
	}

	for(int i=0; i < num_threads; i++){
		thread0[i]->join();
	}

	// for(int i=0; i < num_threads; i++){
	// 	thread0[i] = new thread_numa<0>(QueueTest,i, num_threads);
	// }

	// for(int i=0; i < num_threads; i++){
	// 	thread0[i]->join();
	// }

	// for(int i=0; i < num_threads; i++){
	// 	thread0[i] = new thread_numa<0>(BinarySearchTest,i, num_threads);
	// }

	// for(int i=0; i < num_threads; i++){
	// 	thread0[i]->join();
	// }

	// for(int i=0; i < num_threads; i++){
	// 	thread0[i] = new thread_numa<0>(LinkedListTest,i, num_threads);
	// }

	// for(int i=0; i < num_threads; i++){
	// 	thread0[i]->join();
	// }

	global_cleanup();
}