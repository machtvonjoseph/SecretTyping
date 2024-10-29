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
    # # For example: ls -l
    ./build/bin/clang-tool  --numa input/numa-mt-Data-Structures/Examples/main.cpp input/numa-mt-Data-Structures/Examples/TestSuite.cpp -- -I input/numa-mt-Data-Structures/Node/include/ -I input/numa-mt-Data-Structures/Stack/include/   -I input/numa-mt-Data-Structures/Queue/include/ -I input/numa-mt-Data-Structures/BinarySearch/include/  -I input/numa-mt-Data-Structures/LinkedList/include -I../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma


    ./build/bin/clang-tool  --cast output/numa-mt-Data-Structures/Examples/main.cpp output/numa-mt-Data-Structures/Examples/TestSuite.cpp -- -I output/numa-mt-Data-Structures/Node/include/ -I output/numa-mt-Data-Structures/Stack/include/ -I output/numa-mt-Data-Structures/Queue/include/ -I output/numa-mt-Data-Structures/BinarySearch/include/  -I output/numa-mt-Data-Structures/LinkedList/include -I../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma

    # ./bin/clang-tool  --numa ../input/Data-Structures/Examples/main.cpp ../input/Data-Structures/Examples/TestSuite.cpp -- -I ../input/Data-Structures/Node/include/ -I ../input/Data-Structures/Stack/include/ -I ../input/Data-Structures/Queue/include -I ../input/Data-Structures/BinarySearch/include -I ../input/Data-Structures/LinkedList/include -I../../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma



    ;;
    
  dummy)
    echo "Running dummy"
    # Place the actual command for 'command2' here
    # For example: df -h
    ./bin/clang-tool --numa ../input/Dummy/dummyimpl.cpp -- -I../numaLib/ -I/usr/local/lib/clang/20/include/
    ;;
  
  Exprs)
    echo "Running Exprs"
    # Place the actual command for 'command2' here
    # For example: df -h

    ./build/bin/clang-tool  --numa input/Exprs/Examples/main.cpp input/Exprs/Examples/TestSuite.cpp -- -I input/Exprs/Node/include/ -I input/Exprs/Stack/include/ -I input/Exprs/Queue/include/ -I input/Exprs/BinarySearch/include/  -I input/Exprs/LinkedList/include -I../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma


    ./build/bin/clang-tool  --cast output/Exprs/Examples/main.cpp output/numa-mt-Data-Structures/Examples/TestSuite.cpp -- -I output/Exprs/Node/include/ -I output/Exprs/Stack/include/ -I output/Exprs/Queue/include/ -I output/Exprs/BinarySearch/include/  -I output/Exprs/LinkedList/include -I../numaLib/ -I/usr/local/lib/clang/20/include/ -lnuma
    ;;
  *)
    echo "Invalid argument. Usage: $0 <DS|dummy|Exprs>"
    exit 1
    ;;
esac