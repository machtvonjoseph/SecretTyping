#include "numatype.hpp"
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
class numa<Node,1>{
public:
numa (): data(0){
}
numa (int initData){
    this->data = initData;
}
numa (int initData, Node * node){
    this->data = initData;
    this->link = node;
}
Node * getLink(){
    return this->link;
}
void setLink(Node * n){
    this->link = n;
}
int getData(){
    return this->data;
}
void setData(int n){
    this->data = n;
}
~numa()
{
	link = nullptr;
}
private:
numa<int,1> data;
numa<Node,1>* link;
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