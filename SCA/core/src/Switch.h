#ifndef SWITCH_
#define SWITCH_

#include "SCA.h"
#include "Component.h"

using namespace std;

class Switch : public Component
{
private:
	int numOfCases;
	int numOfBreaks;
	Component componentClass;
	string condition;
	bool conditionIsUnqualifiedId;
	int startLine;
	vector<string> cases;
	vector<int> caseLines;
	bool hasDefault;
	string iteratorInt;


public:
	Switch();

	void setVariables(Node* rt);

	Node* findNode(string data, Node* start);

	string getComponent();
};

Switch::Switch() : Component()
{
	componentClass.setStatementType(1);
	condition = "";
	conditionIsUnqualifiedId = false;
	numOfCases = 0;
	hasDefault = false;
}

void Switch::setVariables(Node* rt)
{
	componentClass.setStartOfStatement(rt);
	Node* statementStart = componentClass.getStartOfStatement();
	Node* walker;
	Node* walker2;
	Node* walker3;

	//find first line number
	walker = findNode("switch", statementStart);
	startLine = walker->getLineNum();

	//find condition
	walker = findNode("condition", statementStart);
	if (walker == nullptr)
		condition = "No condition was found";
	else
	{
		while (walker->getData() != "unqualifiedId")
		{
			if (walker->getChildCount() != 0)
				walker = walker->getChild(0);
			else
				conditionIsUnqualifiedId = false;
		}
		if (walker->getData() == "unqualifiedId")
		{
			conditionIsUnqualifiedId = true;
			walker = walker->getChild(0);
			condition = walker->getData();
		}
	}

	//find cases
	walker = findNode("statement", statementStart);
	walker = walker->getChild(0);
	walker = walker->getChild(0);
	
	for (int i = 0; i < walker->getChildCount(); i++)
	{
		walker2 = walker->getChild(i);
		if (walker2->getChild(0)->getData() == "labeledStatement")
		{
			walker2 = walker2->getChild(0);
			walker3 = walker2->getChild(0);
			numOfCases++;
			caseLines.push_back(walker3->getLineNum());
			if (walker3->getData() == "case")
			{
				walker3 = findNode("constantExpression", walker2);
				if (walker3 != nullptr)
				{
					while (walker3->getData() != "literal")
						walker3 = walker3->getChild(0);
					cases.push_back(walker3->getData());
				}
			}
			else if (walker3->getData() == "default")
			{
				hasDefault = true;
			}
		}
		else if (walker2->getChild(0)->getData() == "jumpStatement")
		{
			numOfBreaks++;
		}
	}
}


Node* Switch::findNode(string data, Node* start)
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
		findNode(data, nodeIter->getChild(i));
	}
	return nullptr;
}

string Switch::getComponent() {
	return "";
}

#endif