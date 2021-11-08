#ifndef NODE_
#define NODE_
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node {
private:
	string data;
	int lineNum;
	vector<Node*> children;
	Node* parent;

public:
	// Constructors
	Node() : data(""), parent(nullptr), lineNum(0) {}
	Node(string d) : data(d), lineNum(0), parent(nullptr) {}
	Node(string d, Node* p) : data(d), parent(p), lineNum(0) {}
	Node(string d, vector<Node*> ch, Node* p)
	{
		data = d;
		parent = p;
		lineNum = 0;
		for (size_t i = 0; i < ch.size(); i++) {
			children.push_back(ch[i]);
		}
	}

	//Accessor and Mutator Methods
	void setData(string d)
	{
		data = d;
	}

	string getData()
	{
		return data;
	}

	void setChildren(vector<Node*> ch)
	{
		children = ch;
	}

	void addChild(Node* ch)
	{
		children.push_back(ch);
	}

	vector<Node*> getChildren()
	{
		return children;
	}

	Node* getChild(const int& i) {
		return children[i];
	}

	void setParent(Node* p)
	{
		parent = p;
	}

	Node* getParent()
	{
		return parent;
	}

	int getChildCount() {
		return children.size();
	}

	void setLineNum(const int& lineN) {
		lineNum = lineN;
	}

	int getLineNum() {
		return lineNum;
	}

};


#endif // NODE_
