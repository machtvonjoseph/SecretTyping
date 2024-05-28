CC=g++
DIRS=Node Stack 
LINK_DIRS=-L../Stack/bin 
LINK_FLAGS=-lstack -lnuma
STATIC_LINK_FLAGS=../Node/bin/libnode.a ../Stack/bin/libstack.a 
INC_DIRS=-I../Node/include -I../Stack/include -std=c++17 
EXE=./bin/DSExample
OBJS=main.o TestSuite.o

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(STATIC_LINK_FLAGS) -o $(EXE) 

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LINK_DIRS) $(LINK_FLAGS) -o $(EXE) 

main.o: 
	$(CC) -c -g main.cpp

TestSuite.o: TestSuite.hpp TestSuite.cpp
	$(CC) -c $(INC_DIRS) -g TestSuite.cpp 

clean:
	rm *.o $(EXE)
