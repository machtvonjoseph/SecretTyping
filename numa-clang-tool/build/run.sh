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
    ./bin/clang-tool ../input/Data-Structures/Examples/main.cpp ../input/Data-Structures/Examples/TestSuite.cpp -- -I ../input/Data-Structures/Node/include/ -I ../input/Data-Structures/Stack/include/  -I../numaLib/ -I/usr/local/lib/clang/18/include/ -lnuma
    ;;
  dummy)
    echo "Running dummy"
    # Place the actual command for 'command2' here
    # For example: df -h
    ./bin/clang-tool ../input/dummyimpl.cpp -- -I../numaLib/ -I/usr/local/lib/clang/18/include/
    ;;
  *)
    echo "Invalid argument. Usage: $0 <DS|dummy>"
    exit 1
    ;;
esac
