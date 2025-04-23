CC=clang++
DIRS=Node Stack
HOME_DIR :=/home/kidus
LINK_DIRS=-L../Stack/bin

LINK_FLAGS=-std=c++20 

STATIC_LINK_FLAGS=../Node/bin/libnode.a ../Stack/bin/libstack.a
INC_DIRS= -I../include/  -I$(HOME_DIR)/SecretTyping/secretLib/

EXE=./bin/DSExample
OBJS=main.o TestSuite.o

UMF =
FLAGS = -fno-omit-frame-pointer

ifdef DEBUG
	FLAGS += -DDEBUG
endif


# $(EXE): $(OBJS)
# 	$(CC) $(OBJS) -o $(EXE) 
# -DPIN_INIT=1 
all: $(EXE) $(TESTEXE)

$(TESTEXE): $(TESTOBJS)
	$(CC) $(TESTOBJS) $(INC_DIRS) $(LINK_FLAGS) -o $(TESTEXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LINK_FLAGS) -o $(EXE) 

main.o: 
	$(CC) -c -O3 -g -std=c++20  $(INC_DIRS) $(FLAGS) -D_NODE_HPP=1  main.cpp 
	

TestSuite.o: TestSuite.hpp TestSuite.cpp
	$(CC) -c  -O3 -g -std=c++20 $(INC_DIRS) $(FLAGS)  TestSuite.cpp 


clean:
	rm *.o $(EXE)
