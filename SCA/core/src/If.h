#ifndef IF_
#define IF_

#include "SCA.h"
#include "Component.h"

using namespace std;

class If : Component
{
private:
	int elses;
	vector<string> conditions;	//0 will always be first if
	vector<string> conditionsL;
	vector<string> conditionsR;
	vector<bool> conditionsLunqualifiedId;
	vector<bool> conditionsRunqualifiedId;
	Component componentClass;
	vector<int> startLines;
	//vector<int> endLines;

public:
	If();
	void setVariables(Node* rt);
	Node* find(string data, Node* start);
	string getComponent();
};

If::If()
{
	componentClass.setStatementType(1);
}

void If::setVariables(Node* rt)
{
	componentClass.setStartOfStatement(rt);
	Node* statementStart = componentClass.getStartOfStatement();
	Node* walker;
	Node* walker2;

	//find first line number
	walker = find("if", statementStart);
	startLines.push_back(walker->getLineNum());

	//find the condition
	walker = find("condition", statementStart);
	if (walker == nullptr)
		conditions.push_back("No condition was found");
	else
	{
		while (walker->getChildCount() == 1)
			walker = walker->getChild(0);
		if (walker->getChildCount() == 3)
		{
			//get left side of condtion
			walker2 = walker->getChild(0);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionsLunqualifiedId.push_back(true);
			else
				conditionsLunqualifiedId.push_back(false);
			conditionsL.push_back(walker2->getData());

			//get comparison operator
			conditions.push_back(walker->getChild(1)->getData());

			//get right side of condition
			walker2 = walker->getChild(2);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionsRunqualifiedId.push_back(true);
			else
				conditionsRunqualifiedId.push_back(false);
			conditionsR.push_back(walker2->getData());
		}
		else if (walker->getChildCount() == 4)
		{
			//get left side of condtion
			walker2 = walker->getChild(0);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionsLunqualifiedId.push_back(true);
			else
				conditionsLunqualifiedId.push_back(false);
			conditionsL.push_back(walker2->getData());

			//get comparison operator
			conditions.push_back(walker->getChild(1)->getData());
			conditions[0] += walker->getChild(2)->getData();

			//get right side of condition
			walker2 = walker->getChild(2);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionsRunqualifiedId.push_back(true);
			else
				conditionsRunqualifiedId.push_back(false);
			conditionsR.push_back(walker2->getData());
		}
	}

	while (statementStart->getChildCount() == 7)	//7 children means there's an else
	{
		elses++;

		walker = find("else", statementStart);
		startLines.push_back(walker->getLineNum());


		statementStart = statementStart->getChild(6);
		if (statementStart->getChild(0)->getData() == "selectionStatement")	//means there's another if
		{
			statementStart = statementStart->getChild(0);

			//find the condition
			walker = find("condition", statementStart);
			if (walker == nullptr)
				conditions.push_back("No condition was found");
			else
			{
				while (walker->getChildCount() == 1)
					walker = walker->getChild(0);
				if (walker->getChildCount() == 3)
				{
					//get left side of condtion
					walker2 = walker->getChild(0);

					while (walker2->getChildCount() == 1)
						walker2 = walker2->getChild(0);

					if (walker2->getParent()->getData() == "unqualifiedId")
						conditionsLunqualifiedId.push_back(true);
					else
						conditionsLunqualifiedId.push_back(false);
					conditionsL.push_back(walker2->getData());

					//get comparison operator
					conditions.push_back(walker->getChild(1)->getData());

					//get right side of condition
					walker2 = walker->getChild(2);

					while (walker2->getChildCount() == 1)
						walker2 = walker2->getChild(0);

					if (walker2->getParent()->getData() == "unqualifiedId")
						conditionsRunqualifiedId.push_back(true);
					else
						conditionsRunqualifiedId.push_back(false);
					conditionsR.push_back(walker2->getData());
				}
				else if (walker->getChildCount() == 4)
				{
					//get left side of condtion
					walker2 = walker->getChild(0);

					while (walker2->getChildCount() == 1)
						walker2 = walker2->getChild(0);

					if (walker2->getParent()->getData() == "unqualifiedId")
						conditionsLunqualifiedId.push_back(true);
					else
						conditionsLunqualifiedId.push_back(false);
					conditionsL.push_back(walker2->getData());

					//get comparison operator
					conditions.push_back(walker->getChild(1)->getData());
					conditions[elses] += walker->getChild(2)->getData();

					//get right side of condition
					walker2 = walker->getChild(2);

					while (walker2->getChildCount() == 1)
						walker2 = walker2->getChild(0);

					if (walker2->getParent()->getData() == "unqualifiedId")
						conditionsRunqualifiedId.push_back(true);
					else
						conditionsRunqualifiedId.push_back(false);
					conditionsR.push_back(walker2->getData());
				}
			}

		}
	}
}


Node* If::find(string data, Node* start)
{
	Node* nodeIter = start;
	string iteratorInt;

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

string If::getComponent() {
	return "";
}

#endif