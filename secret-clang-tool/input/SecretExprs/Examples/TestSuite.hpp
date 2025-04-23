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
#include "secrettype.hpp"

using namespace std;

void secret_Stack_init();

/*!
 * \brief Test function for the Stack classes
 * 
 * Invokes the functions of the Stack class 
 * (pop, push, ctor, dtor, display)
 * 
 */

void StackTest();

#endif 