# Project Name

A brief description of the project, including its purpose, features, and key technologies used.

## Table of Contents

- [Project Structure](#project-structure)
- [Building](#building)
- [Usage](#usage)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [License](#license)

## Project Structure

There are 5 folders in total in this repo. [Data-Structures](#data-structures), [mt-Data-Structures] and [numa-mt-Data-Structures] all contain the same variations of data structures where the first is a single non-numa threaded group of data structures, the second is a multi-non-numa threaded group of data structures and the later is multi-numa threaded data structures. Each of these directories hold a [numa-DS] sub directory in them that have a numa-typed data structures in them. 

The [numaLib] directory holds the libraries for our ```numa<Type,NodeId>``` type and ```thread_numa<NodeId>``` types

The [numa-clang-tool] holds our source to source transformation tool build using the clang front end.

### Data Structures
Data-Structures holds four types of data structures we will use for benchmarking; a binary search tree, a linked list, a queue and a stack.
To compile and run the operations specified on these data structures run 
```bash 
make clean
make 
./Examples/DSExample
```
This is of no interest for the overall project but the implementation files are in the 
**Examples/** directory.

### mt-Data-Structures
This directory holds the same structure as [Data-Structures](#data-structures) but its implementation is multi threaded, i.e multiple threads will do the operations specified on the data structure. To run this you will have to pass the number of threads you want to use as a parameter
To build and run the operations on the data structures using 5 threads for example you run
```bash 
make clean
make 
./Examples/bin/DSExample 5
```

### numa-mt-Data-Structures
This is the same as [mt-Data-Structures](#mt-data-structures) except instead of using the threads library, we use the numa_threads library. Therefore, instead of ```std::threads``` we have ```thread_numa<0>```. This means that all the threads spawned are on numa node 0. To build and run it is the same [mt-Data-Structures](#mt-data-structures). 

```bash 
make clean
make 
./Examples/bin/DSExample 5
```

### numaed-DS 
All the three directories above hold a directory **numaed-DS** within them. Disregard this directory for now.

### numaLib
This directory holds the header file for the ```numa<Type, NodeID>``` type and the ```thread_numa<NodeID>```type. All files that make use of the ```numa``` type must include these two files.

### numa-clang-tool
This has all the necessary clang-tool source code. [INSERT LINK TO NOTION]

## Building

1. Make sure the main repository is in your home directory. 
2. To compile the clang tool, 
```bash
    cd NUMATYPING/numa-clang-tool
    mkdir build
    cd build 
    cmake ..
    cmake --build .
```
NB: You only have to do this the first time. To compile any changes made to the tool you can just do 
```bash
    cd NUMATYPING/numa-clang-tool/build
    cmake --build .
```

## Usage
To use the source to source transformation tool you will have to have all your source code in the **input/** directory. Currently, **input/** has four benchmarks; **Data-Structures/**, **Dummy/** , **numa-mt-Data-Structures/** and **Point/**. 
The **Dummy/** and **Point/** benchmarks are very simple dummy benchmarks used for debugging.

To run the **numa-mt-Data-Structures/** benchmark for examples (This is what we are interested in the most), we have a script **run.sh** the **numa-clang-tool/** directory and you can invoke the script with the DS argument, 
```bash
./run.sh DS
```
(If you want to run the dummy benchmark instead of the DS, you can use dummy. The other benchmarks are not supported through the scipt yet.)

The script with the DS argument takes in the **input/numa-mt-Data-Structures/** benchmark, i.e numa-multi-threaded data structures, and it numa types all the allocations of the four data structures within and copies all numa allocation mechanisms within the class. The transformed code will be put in **output2/numa-mt-Data-Structures**. There will be an intermediate product of running this script that only handles recursive typing and copying numa allocators in the **output/numa-mt-Data-Structures** directory.

To verify the output works do 
```bash
cd output2/numa-mt-data-structures
make clean
make DEBUG=1
./Examples/bin/DSExample 5
```

The argument 5 is the number of threads to spawn. If it compiles and it runs correctly, the tool works.


