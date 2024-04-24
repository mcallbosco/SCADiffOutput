#ifndef COMPONENT
#define COMPONENT

#include "SCA.h"
#include "Tree.h"
#include <regex>

using namespace std;

class Component 
{
private:
	Node* treeRootNode;
	Node* componentRootNode;

	// used for recursively searching tree for a specific node
	Node* foundNode;

	int statementTypeint;
	string statementType;
	string componentType;
	vector<int> startLine;
	int startLineCount;
	vector<int> endLine;
	int endLineCount;
	bool correctComponent;
	vector<string> codeSmells;

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
	void setStartOfStatement(Node* rt);

	//returns the Node which its data contains the statementType, so the start of the component.
	Node* getStartOfStatement();

	//returns vector of pointers to all nodes in the statement.
	vector<Node*> getStatementInstances();

	//goes to the bottom right node, start from start node* and sets endLine.
	void addAnEndLine(Node* start);

	//returns endLine.
	vector<int> getEndLine();

	//returns amount of endLines.
	int getEndLineCount();

	//sets startLine as int start.
	void addAnStartLine(int start);

	//returns startLine.
	vector<int> getStartLine();

	//return first instance of start line
	int getFirstStartLine();

	//returns amount of startLines.
	int getStartLineCount();

	//sets if the component is correct or not.
	void setCorrectComponent(bool correct);

	//returns if component is correct or not.
	bool getCorrectComponent();

	//searches from start node to find data, returns first node with the same data in it.
	bool findNode(string data, Node* start);

	//getComponent => will be overridden in our component classes, returns a string in html format
	string getComponent();

	// check Component, implemented in derived classes to check componenet's correctness.
	void checkComponent();

	// set a code smell
	void setCodeSmell(string aSmell);

	// get code smells
	vector<string> getCodeSmells();

	// checks if a string is an integer
	bool isInteger(const string& str);

	// check if a string is numeric including double and decimal
	bool isNumeric(string str);

	// accessor Methods
	Node* getTreeRootNode();
	Node* getComponentRootNode();
	Node* getFoundNode();
};

Component::Component() {
	componentRootNode = nullptr;
	treeRootNode = nullptr;
	foundNode = nullptr;
}

Component::Component(Node* compRtPtr) {
	componentRootNode = compRtPtr;
	treeRootNode = nullptr;
	foundNode = nullptr;
	correctComponent = true;
	startLineCount = 0;
	endLineCount = 0;
}

Component::Component(Node* compRtPtr, Node* treeRtPtr) {
	componentRootNode = compRtPtr;
	treeRootNode = treeRtPtr;
	foundNode = nullptr;
	correctComponent = true;
	startLineCount = 0;
	endLineCount = 0;
}

Node* Component::getTreeRootNode() {
	return treeRootNode;
}

Node* Component::getComponentRootNode() {
	return componentRootNode;
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

void Component::addAnEndLine(Node* start)
{
	Node* nodeIter = start;

	while (nodeIter->getChildCount() != 0)
		nodeIter = nodeIter->getChild(nodeIter->getChildCount() - 1);

	endLine.push_back(nodeIter->getLineNum());
	endLineCount++;
}

vector<int> Component::getEndLine() {
	return endLine;
}

int Component::getEndLineCount() {
	return endLineCount;
}

void Component::addAnStartLine(int start) {
	startLine.push_back(start);
	startLineCount++;
}

vector<int> Component::getStartLine() {
	std::cout << "Start line: " << std::endl;
	return startLine;
}

int Component::getStartLineCount() {
	return startLineCount;
}

void Component::setCorrectComponent(bool correct) {
	correctComponent = correct;
}

bool Component::getCorrectComponent() {
	return correctComponent;
}

Node* Component::getFoundNode() {
	return foundNode;
}

bool Component::findNode(string data, Node* start)
{
	Node* nodeIter = start;

	if (nodeIter->getData() == data) {
		foundNode = nodeIter;
		return true;
	}
	for (int i = 0; i < nodeIter->getChildCount(); i++) {
		if (findNode(data, nodeIter->getChild(i)))
			return true;
	}
	return false;
}

string Component::getComponent() {
	// override this function
	// will return a string formatted for html
	// use <br /> for line breaks instead of 'endl' or '\n'
	// Base function returns an empty string
	return "";
}

void Component::checkComponent() {
	// implemenation left to dervied classes.
}

void Component::setCodeSmell(string aSmell) {
	codeSmells.push_back(aSmell);
}

vector<string> Component::getCodeSmells() {
	return codeSmells;
}

bool Component::isInteger(const string& str) {
	for (char const &c : str) {
		if (isdigit(c) == 0)
			return false;
	}
	return true;
}

bool Component::isNumeric(string str) {
	regex e ("[+-]?[0-9]+\\.?[0-9]*");

	if (regex_match(str, e))
		return true;
	
	return false;
}

int Component::getFirstStartLine() {
	if (startLine.size() > 0)
		return startLine[0];
	else
		return 0;
}

#endif // !COMPONENT
