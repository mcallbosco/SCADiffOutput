#ifndef IF_
#define IF_

#include "SCA.h"
#include "Component.h"

using namespace std;

class If : Component
{
private:
	int elses;
	bool isAnElse;
	vector<string> conditions;	//0 will always be first if
	vector<string> conditionsL;
	vector<string> conditionsR;
	vector<bool> conditionsLunqualified;
	vector<bool> conditionsRunqualified;
	Component componentClass;
	vector<int> startLines;
	vector<int> endLines;

public:
	If();
	void setVariables(Tree* rt);
	Node* find(string data, Node* start);
};

If::If()
{
	componentClass.setStatementType(1);
	
}


Node* If::find(string data, Node* start)
{
	Node* nodeIter = start;

	if (nodeIter->getData() == data)
	{
		iteratorInt = nodeIter->getData();
		return nodeIter;
	}

	if (nodeIter->getChildCount() == 0)
		return nullptr;

	int totalChildren = nodeIter->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		find(data, nodeIter->getChild(i));
	}
}

#endif