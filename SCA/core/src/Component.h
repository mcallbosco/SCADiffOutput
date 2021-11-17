#ifndef COMPONENT
#define COMPONENT

#include "SCA.h"

using namespace std;

class Component 
{
private:
	int statementTypeint;
	string statementType;
	Node* startOfStatement;
	string componentType;
	vector<Node*> statementInstances;
public:

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
};

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

#endif // !COMPONENT