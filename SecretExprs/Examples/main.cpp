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
// #include "numatype.hpp"
// #include "numathreads.hpp"
#include <cmath>
#include <getopt.h>
#include <chrono>
#include <iomanip>
#include <unordered_map>

using namespace std;

std::string DS_name;

void print_function(std::string DS_name){
	auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::cout<<std::put_time(local_time, "%Y-%m-%d") << ", ";
	std::cout<<std::put_time(local_time, "%H:%M:%S") <<", ";
	std::cout<<DS_name << ", ";
	std::cout<<endl;
}


int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"DS_name", required_argument, nullptr, 's'},       // --DS_name=STACK
		{nullptr, 0, nullptr, 0}                            // End of array
	};

 	int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "s:", long_options, &option_index)) != -1) {
        switch (opt) {
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

	print_function(DS_name);

	if(DS_name == "stack"){
		secret_Stack_init();
		StackTest();
	}
	else{
		cout<<"Invalid Data Structure"<<endl;
	}
}
