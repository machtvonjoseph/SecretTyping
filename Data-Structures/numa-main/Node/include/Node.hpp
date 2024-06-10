#ifndef _NODE_HPP_
#define _NODE_HPP_


#include "numatype.hpp"



class Node;
template<>
class numa<Node, 1>{
private:
	numa<int,1> data;
	numa<Node*,1> link;
public:
	numa(){
		link = nullptr;
	}
	numa(int initData){
		data = initData;
	}
	Node *getLink(){ return link; }
	~numa(){
	link = NULL;
	}

};


class Node
{
private:
	int data;
	Node *link;

public:
	Node() : data(0)
	{}
	Node(int);
	Node(int, Node*);
	Node *getLink(){ return link; }
	void setLink(Node *n) { link = n; }

	int getData(){ return data; }
	void setData(int n){ data = n; }

	virtual ~Node();
};

Node::Node(int initData)
{
	data = initData;
}

Node::~Node()
{
	link = nullptr;
}

Node::Node(int initData, Node *node)
{
	data = initData;
	link = node;
}

#endif //_NODE_HPP_