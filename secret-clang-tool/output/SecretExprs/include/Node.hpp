#ifdef UMF 
	                #include "numatype.hpp"
	                #include <umf/mempolicy.h>
	                #include <umf/memspace.h>
                    #include "utils_examples.h"
                    #include <stdio.h>
                    #include <string.h>
                #endif
                #include "secrettype.hpp"
                
#ifndef _NODE_HPP_
#define _NODE_HPP_

class Node
{
private:
	int data;
	Node *link;

public:
	Node() : data(0)
	{}
	Node(int initData);
	Node(int, Node*);
	virtual Node *getLink();
	virtual void setLink(Node *n);

	virtual int getData();
	virtual void setData(int n);

	virtual ~Node();
};

template<>
class secret<Node>{
//add your secure memory allocator code herepublic:
secret (): data(0){
}
secret (int initData){
    this->data = initData;
}
secret (int initData, Node * node){
    this->data = initData;
    this->link = node;
}
virtual Node * getLink(){
    return this->link;
}
virtual void setLink(Node * n){
    this->link = n;
}
virtual int getData(){
    return this->data;
}
virtual void setData(int n){
    this->data = n;
}
virtual ~secret()
{
	link = nullptr;
}
private:
numa<int> data;
secret<Node*> link;
};


Node::Node(int initData)
{
	data = initData;
}

Node::Node(int initData, Node *node)
{
	data = initData;
	link = node;
}
 
Node* Node::getLink()
{
	return link;
}
void Node::setLink(Node *n)
{
	link = n;
}

int Node::getData()
{
	return data;
}

void Node::setData(int n)
{
	data = n;
}

Node::~Node()
{
	link = nullptr;
}



#endif //_NODE_HPP_
