#ifndef COMPONENT
#define COMPONENT

#include "SCA.h"

using namespace std;

class Component 
{
private:
	Node* treeRootNode;
	Node* componentRootNode;

	int statementTypeint;
	string statementType;
	Node* startOfStatement;
	string componentType;
	vector<Node*> statementInstances;
	vector<int> startLine;
	int startLineCount;
	vector<int> endLine;
	int endLineCount;
public:
	// Default Constructor
	Component();
	// Contructor with component root node
	Component(Node* compRtPtr);
	// Constructor with component root node and tree root node
	Component(Node* compRtPtr, Node* treeRtPtr);
	//Pass in 0 for 'iterationStatement' and 1 for 'selectionStatement'.
	void setStatementType(int type);

	//returns statementType in string form.
	string getStatementType();

	//Uses filltokenNodeVector() and statementType to set the start of this component. Also sets componentType off first child node.
	void setStartOfStatement(Tree* rt);

	//returns the Node which its data contains the statementType, so the start of the component.
	Node* getStartOfStatement();

	//returns vector of pointers to all nodes in the statement.
	vector<Node*> getStatementInstances();

	//goes to the bottom right node start from start node* and sets endLine.
	void addAnEndLine(Node* start);

	//returns endLine.
	vector<int> getEndLine();

	//returns amount of endLines.
	int getEndLineCount();

	//sets startLine as int start.
	void addAnStartLine(int start);

	//returns startLine.
	vector<int> getStartLine();

	//returns amount of startLines.
	int getStartLineCount();

	//searches from start node to find data, returns first node with the same data in it.
	Node* find(string data, Node* start);
};

Component::Component() {
	componentRootNode = nullptr;
	treeRootNode = nullptr;
}

Component::Component(Node* compRtPtr) {
	componentRootNode = compRtPtr;
	treeRootNode = nullptr;
}

Component::Component(Node* compRtPtr, Node* treeRtPtr) {
	componentRootNode = compRtPtr;
	treeRootNode = treeRtPtr;
}

void Component::setStatementType(int type){
	statementTypeint = type;

	if (type == 0)
		statementType = "iterationStatement";
	else if (type == 1)
		statementType = "selectionStatement";
	else
		statementType = "Incorrect input for setStatementType()";
}

string Component::getStatementType() {
	return statementType;
}

void Component::setStartOfStatement(Tree* rt)
{
	startOfStatement = rt->getRoot();
	rt->filltokenNodeVector(statementType, startOfStatement);
	statementInstances = rt->getTokenNodes();

	componentType = startOfStatement->getChild(0)->getData();
}

Node* Component::getStartOfStatement(){
	return startOfStatement;
}

vector<Node*> Component::getStatementInstances() {
	return statementInstances;
}

void Component::addAnEndLine(Node* start)
{
	Node* nodeIter = start;

	while (nodeIter->getChildCount() != 0)
		nodeIter = nodeIter->getChild(nodeIter->getChildCount() - 1);

	endLine[endLineCount] = nodeIter->getLineNum();
	endLineCount++;
}

vector<int> Component::getEndLine() {
	return endLine;
}

int Component::getEndLineCount() {
	return endLineCount;
}

void Component::addAnStartLine(int start) {
	startLine[startLineCount] = start;
	startLineCount++;
}

vector<int> Component::getStartLine() {
	return startLine;
}

int Component::getStartLineCount() {
	return startLineCount;
}

Node* Component::find(string data, Node* start)
{
	Node* nodeIter = start;

	if (nodeIter->getData() == data)
	{
		return nodeIter;
	}

	if (nodeIter->getChildCount() == 0)
		return nullptr;

	int totalChildren = nodeIter->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		find(data, nodeIter->getChild(i));
	}
}

#endif // !COMPONENT