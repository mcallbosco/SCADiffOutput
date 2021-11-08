#include "Tree.h"

bool Tree::isEmpty() {
	return nodeCount == 0;
}

void Tree::addParent(const string& newItem) {
	Node* newNodePtr = new Node(newItem);

	//if empty, make root
	if (isEmpty()) {
		newNodePtr->setParent(nullptr);
		root = newNodePtr;
		traveler = root;
	}
	else {
		newNodePtr->setParent(traveler);
		traveler->addChild(newNodePtr);
		traveler = newNodePtr;
	}
	nodeCount++;
}

void Tree::addLeaf(const string& newItem) {
	//don't move traveler to leaf
	Node* newNodePtr = new Node(newItem, traveler);
	traveler->addChild(newNodePtr);
	nodeCount++;
}

Node* Tree::removeParent(Node* nodePtr) {
	// get pointers to child node and parent node
	Node* child = nodePtr->getChild(0);
	Node* parent = nodePtr->getParent();
	
	// make temporary vector and fill with the child node
	// node to be removed will always have exactly 1 child
	vector<Node*> temp;
	temp.push_back(child);
	
	// set child node's parent
	child->setParent(parent);
	
	// set parent node's children
	parent->setChildren(temp);

	// delete current node
	delete nodePtr;

	// decrement node count
	nodeCount--;

	// return child pointer to continue travesal of condensing process
	return parent;
}

void Tree::popBackToParent() {
	if (traveler->getParent() != nullptr) {
		traveler = traveler->getParent();
	}
	else { traveler = root; }
}

string Tree::LevelOrderTraversaltoString() {
	Node* rt = root;
	string treeString;
	if (isEmpty()) {
		cout << "List is empty!" << endl;
	}
	else {
		queue<Node*> q;
		q.push(rt);
		while (!q.empty()) {
			int n = q.size();

			while (n > 0) { 
				//if node has children
				//Dequeue an item from queue and print it
				Node* p = q.front();
				q.pop();
				treeString += p->getData() + " ";

				//Enqueue all children of the dqueued item
				for (int i = 0; i < p->getChildCount(); i++) {
					q.push(p->getChild(i));
				}
				n--;

			}
			treeString += "<br/>";
		}
	}
	return treeString;
}

void Tree::printLevelOrderTraversal() {
	Node* rt = root;
	if (isEmpty()) {
		cout << "List is empty!" << endl;
		return;
	}
	else {
		queue<Node*> q;
		q.push(rt);
		while (!q.empty()) {
			int n = q.size();

			while (n > 0) {
				//if node has children
				//Dequeue an item from queue and print it
				Node* p = q.front();
				q.pop();
				cout << p->getData() + " ";

				//Enqueue all children of the dqueued item
				for (int i = 0; i < p->getChildCount(); i++) {
					q.push(p->getChild(i));
				}
				n--;

			}
			cout << endl;
		}
	}
}

string Tree::getNextNtokens(Node* rt, int n) {
	// pop back to appropriate parent.
	while (rt->getChildCount() <= 1) {
		rt = rt->getParent();
	}

	//clear the nextNtokens string
	nextNtokens = "";
	nextTokenCount = 0;
	_getNextNtokens(rt, n);
	return getNextNtokensString();
}

void Tree::_getNextNtokens(Node* rt, int n) {
	if (nextTokenCount == n) {
		return;
	}

	if (rt->getChildCount() == 0) {
		string temp = rt->getParent()->getData();
		temp = trim(temp);
		if (temp == "unqualifiedId" || temp == "literal") {
			nextNtokens += rt->getParent()->getData();
		}
		else {
			nextNtokens += rt->getData();
		}
		nextTokenCount++;
		return;
	}

	int totalChildren = rt->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		_getNextNtokens(rt->getChild(i), n);
	}
}

string Tree::trim(const string& line) {
	const char* WhiteSpace = " \t\v\r\n";
	size_t start = line.find_first_not_of(WhiteSpace);
	size_t end = line.find_last_not_of(WhiteSpace);
	return start == end ? string() : line.substr(start, end - start + 1);
}

void Tree::filltokenNodeVector(const string& token, Node* rt) {
	Node* nodeIter = rt;
	if (nodeIter->getChildCount() == 0) {
		return;
	}

	int totalChildren = nodeIter->getChildCount();
	
	for (int i = 0; i < totalChildren; i++) {
		if (nodeIter->getChild(i)->getData() == token) {
			tokenNodes.push_back(nodeIter->getChild(i));
		}
		filltokenNodeVector(token, nodeIter->getChild(i));
	}
}

void Tree::printInOrder(Node* aNode) {
	Node* nodeIter = aNode;
	if (nodeIter->getChildCount() == 0) {
		cout << nodeIter->getData() << " ";
		return;
	}

	int totalChildren = nodeIter->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		printInOrder(nodeIter->getChild(i));
	}

}

void Tree::printNodeCount() {
	cout << "Node count: " << nodeCount << endl;
}

void Tree::printRootData() {
	cout << "Root data: " << root->getData() << endl;
}

void Tree::printTravelerData() {
	cout << "Traveler data: " << traveler->getData() << endl;
}

int Tree::getNodeCount() {
	return nodeCount;
}

string Tree::getRootData() {
	return root->getData();
}

string Tree::getTravelerData() {
	return traveler->getData();
}

Node* Tree::getRoot() {
	return root;
}

void Tree::clearTokenNodes() {
	tokenNodes.clear();
}

vector<Node*> Tree::getTokenNodes() {
	return tokenNodes;
}

Node* Tree::getTokenNode(const int& i) {
	return tokenNodes[i];
}

Node* Tree::getTravelerNode() {
	return traveler;
}

string Tree::getNextNtokensString() {
	return nextNtokens;
}