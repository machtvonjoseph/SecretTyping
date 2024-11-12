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

using namespace std;

void DS_init();
void sync_init(int num_threads);

/*!
 * \brief Test function for the Stack classes
 * 
 * Invokes the functions of the Stack class 
 * (pop, push, ctor, dtor, display)
 * 
 */
void* StackTest(int t_id, int num_threads);

/*!
 * \brief Test function for the Stack classes
 * 
 * Invokes the functions of the Stack class 
 * (pop, push, ctor, dtor, display)
 * 
 */
void QueueTest(int t_id, int num_threads);

/*!
 * \brief Test function for the Stack classes
 * 
 * Invokes the functions of the Stack class 
 * (pop, push, ctor, dtor, display)
 * 
 */
void BinarySearchTest(int t_id, int num_threads);

/*!
 * \brief Test function for the Stack classes
 * 
 * Invokes the functions of the Stack class 
 * (pop, push, ctor, dtor, display)
 * 
 */
void LinkedListTest(int t_id, int num_threads);

void global_cleanup();

#endif //_TESTSUITE_HPP_