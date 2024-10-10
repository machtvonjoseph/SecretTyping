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



int main(int argc, char *argv[])
{
	int num_threads = std::stoi(argv[1]);
	std::vector <std::thread*> threads(num_threads);
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "Welcome to the Multi Threaded Test Suite for Data Structures" << std::endl;

	std::cout << "Initializing Test Suite with " << num_threads << " threads." << std::endl;
	threads.resize(num_threads);

	DS_init();
	sync_init(num_threads);
	
	// for(int i=0; i < num_threads; i++){
	// 	threads[i] = new std::thread(StackTest,i, num_threads);
	// }

	// for(int i=0; i < num_threads; i++){
	// 	threads[i]->join();
	// }

	size_t i;
	for(i=1; i < num_threads; i++){
		threads[i] = new thread(StackTest, i+1, num_threads);
	}
	i =1;
	StackTest(i, num_threads);
	for(size_t i=1; i < num_threads; i++){
		cout << "Joining thread " << i << endl;
        threads[i]->join();
        printf("Thread %zu joined\n", i);
        delete threads[i];
    }

	threads.resize(num_threads);
	cout<<threads.size()<<endl;

	for(i=1; i < num_threads; i++){
		cout<<"Creating threads for Queue"<<i<<endl;
		threads[i] = new thread(QueueTest, i+1, num_threads);
	}
	i =1;
	QueueTest(i, num_threads);

	for(size_t i=1; i < num_threads; i++){
		cout << "Joining thread " << i << endl;
		threads[i]->join();
		printf("Thread %zu joined\n", i);
		delete threads[i];
	}


	for(i=1; i < num_threads; i++){
		threads[i] = new thread(BinarySearchTest, i+1, num_threads);
	}
	i =1;
	BinarySearchTest(i, num_threads);

	for(size_t i=1; i < num_threads; i++){
		cout << "Joining thread " << i << endl;
		threads[i]->join();
		printf("Thread %zu joined\n", i);
		delete threads[i];
	}

	for(i=1; i < num_threads; i++){
		threads[i] = new thread(LinkedListTest, i+1, num_threads);
	}
	i =1;
	LinkedListTest(i, num_threads);

	for(size_t i=1; i < num_threads; i++){
		cout << "Joining thread " << i << endl;
		threads[i]->join();
		printf("Thread %zu joined\n", i);
		delete threads[i];
	}

	global_cleanup();


	return 0;
}