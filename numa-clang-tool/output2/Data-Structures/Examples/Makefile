CC=clang++
DIRS=Node Stack Queue BinarySearch
LINK_DIRS=-L../Stack/bin -L../Queue/bin -L../BinarySearch/bin
LINK_FLAGS=-lnuma
STATIC_LINK_FLAGS=../Node/bin/libnode.a ../Stack/bin/libstack.a ../Queue/bin/libqueue.a ../BinarySearch/bin/libbinarysearch.a 
INC_DIRS= -I../Node/include -I../Stack/include -I../Queue/include -I../../../numaLib -I../BinarySearch/include -std=c++17 
EXE=./bin/DSExample
OBJS=main.o TestSuite.o

$(EXE): $(OBJS)
	$(CC) $(OBJS)  -o $(EXE) 

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LINK_FLAGS) -o $(EXE) 

main.o: 
	$(CC) -c -g  $(INC_DIRS) -D_NODE_HPP=1 main.cpp

TestSuite.o: TestSuite.hpp TestSuite.cpp
	$(CC) -c $(INC_DIRS) -g TestSuite.cpp 

clean:
	rm *.o $(EXE)
