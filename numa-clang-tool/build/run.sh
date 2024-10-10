#!/bin/bash

# Check if an argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <DS|dummy>"
  exit 1
fi

# Run different commands based on the argument
case "$1" in
  DS)
    echo "Running DS"
    # Place the actual command for 'command1' here
    # For example: ls -l
    ./bin/clang-tool  --numa ../input/mt-Data-Structures/Examples/main.cpp ../input/mt-Data-Structures/Examples/TestSuite.cpp -- -I ../input/mt-Data-Structures/Node/include/ -I ../input/mt-Data-Structures/Stack/include/   -I ../input/mt-Data-Structures/Queue/include/ -I ../input/mt-Data-Structures/BinarySearch/include/  -I ../input/mt-Data-Structures/LinkedList/include -I../../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma


    ./bin/clang-tool  --cast ../output/mt-Data-Structures/Examples/main.cpp ../output/mt-Data-Structures/Examples/TestSuite.cpp -- -I ../output/mt-Data-Structures/Node/include/ -I ../output/mt-Data-Structures/Stack/include/ -I ../output/mt-Data-Structures/Queue/include/ -I ../output/mt-Data-Structures/BinarySearch/include/  -I ../output/mt-Data-Structures/LinkedList/include -I../../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma

    # ./bin/clang-tool  --numa ../input/Data-Structures/numaed-DS/Examples/main.cpp ../input/Data-Structures/numaed-DS/Examples/TestSuite.cpp -- -I ../input/Data-Structures/numaed-DS/Node/include/ -I ../input/Data-Structures/numaed-DS/Stack/include/  -I../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma
    ;;
    
  dummy)
    echo "Running dummy"
    # Place the actual command for 'command2' here
    # For example: df -h
    ./bin/clang-tool --numa ../input/Dummy/dummyimpl.cpp -- -I../numaLib/ -I/usr/local/lib/clang/20/include/
    ;;
  *)
    echo "Invalid argument. Usage: $0 <DS|dummy>"
    exit 1
    ;;
esac
