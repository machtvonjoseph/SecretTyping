CC=clang++
DIRS=Node Stack Queue BinarySearch
HOME_DIR :=$(HOME)
LINK_DIRS=-L../Stack/bin -L../Queue/bin -L../BinarySearch/bin -L../LinkdedList/bin
LINK_FLAGS=-lnuma -std=c++20 -pthread
STATIC_LINK_FLAGS=../Node/bin/libnode.a ../Stack/bin/libstack.a ../Queue/bin/libqueue.a ../BinarySearch/bin/libbinarysearch.a ../LinkdedList/bin/liblinkedlist.a
INC_DIRS= -I../Node/include -I../Stack/include -I../Queue/include -I../BinarySearch/include -I ../LinkedList/include -I$(HOME_DIR)/NUMATyping/numaLib
EXE=./bin/DSExample
OBJS=main.o TestSuite.o

FLAGS = 
ifndef DEBUG
#  FLAGS += -DEBUG
else
	FLAGS += -DDEBUG
endif


$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) 

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LINK_FLAGS) -o $(EXE) 

main.o: 
	$(CC) -c -g -std=c++20  -pthread $(INC_DIRS) $(FLAGS) -D_NODE_HPP=1 main.cpp

TestSuite.o: TestSuite.hpp TestSuite.cpp
	$(CC) -c -O0 -std=c++20  -pthread $(INC_DIRS) $(FLAGS) -g TestSuite.cpp 

clean:
	rm *.o $(EXE)
