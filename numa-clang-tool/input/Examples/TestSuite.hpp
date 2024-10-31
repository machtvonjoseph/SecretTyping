#pragma once
/*! \file TestSuite.hpp
 * \brief Testsuite program which allows for testing of various data structures
 * \author Nii Mante
 * \date 10/28/2012
 *
 */

#ifndef _TESTSUITE_HPP_
#define _TESTSUITE_HPP_


#include <thread>
#include <barrier>
#include <mutex>
#include <iostream>
#include <syncstream>
#include <string>
#include <vector>

using namespace std;


void numa_Stack_init(std::string DS_config, int num_DS, int num_threads);
void numa_Queue_init(std::string DS_config, int num_DS, int num_threads);
void numa_BST_init(std::string DS_config, int num_DS, int num_threads);
void numa_LL_init(std::string DS_config, int num_DS, int num_threads);
void sync_init(int num_threads);

/*!
 * \brief Test function for the Stack classes
 * 
 * Invokes the functions of the Stack class 
 * (pop, push, ctor, dtor, display)
 * 
 */
void StackTest(int t_id, int duration, int node, int64_t num_DS, int num_threads);

void QueueTest(int t_id, int duration, int node, int64_t num_DS, int num_threads);

void BinarySearchTest(int t_id, int duration, int node, int64_t num_DS, int num_threads);

void LinkedListTest(int t_id, int duration, int node, int64_t num_DS, int num_threads);
// /*!
//  * \brief Test function for the Stack classes
//  * 
//  * Invokes the functions of the Stack class 
//  * (pop, push, ctor, dtor, display)
//  * 
//  */
// void* QueueTest(int t_id, int duration, int node, int64_t num_DS, int num_threads);

// /*!
//  * \brief Test function for the Stack classes
//  * 
//  * Invokes the functions of the Stack class 
//  * (pop, push, ctor, dtor, display)
//  * 
//  */
// void* BinarySearchTest(int t_id, int duration, int node, int64_t num_DS, int num_threads);

// /*!
//  * \brief Test function for the Stack classes
//  * 
//  * Invokes the functions of the Stack class 
//  * (pop, push, ctor, dtor, display)
//  * 
//  */
// void* LinkedListTest(int t_id, int duration, int node, int64_t num_DS, int num_threads);

void global_cleanup();

#endif //_TESTSUITE_HPP_