#ifndef TREE_
#define TREE_
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Node.h"
using namespace std;

class Tree {
private:
	int nodeCount;
	Node* root;
	Node* traveler;
	vector<Node*> tokenNodes;
	string nextNtokens = "";
	int nextTokenCount = 0;

public:
	// Constructors
	Tree() : nodeCount(0), root(nullptr), traveler(nullptr) {}
	Tree(Node* rt) : nodeCount(1), root(rt), traveler(rt) {}
	bool isEmpty();

	// Add nodes to tree (parent will have children, leaf will not)
	void addParent(const string& newItem);
	void addLeaf(const string& newItem);

	// Removes nodes from tree
	Node* removeParent(Node* nodePtr);

	// Pop traveler back to parent node (used for tree creation)
	void popBackToParent();
	
	// Return node pointers to nodes that match token
	void filltokenNodeVector(const string& token, Node* rt);

	// Return string of next n tokens
	string getNextNtokens(Node* rt, int n);
	void _getNextNtokens(Node* rt, int n);

	// trim leading and trailing white spaces from string
	string trim(const string& line);
	
	// level-order traversal => writes tree to string for html output
	string LevelOrderTraversaltoString();

	// Print functions:
	void printLevelOrderTraversal();
	void printNodeCount();
	void printRootData();
	void printTravelerData();
	void printInOrder(Node* aNode);
	
	// Accessor & Mutator Functions
	int getNodeCount();
	string getRootData();
	string getTravelerData();
	Node* getRoot();
	void clearTokenNodes();
	vector<Node*> getTokenNodes();
	Node* getTokenNode(const int& i);
	Node* getTravelerNode();
	string getNextNtokensString();

};

#include "Tree.cpp"

#endif // TREE_
