#ifndef IF_
#define IF_

#include "SCA.h"
#include "Component.h"

using namespace std;

class If : public Component
{
private:
	int elses;
	string conditions;	//0 will always be first if
	string conditionsL;
	string conditionsR;
	vector<bool> conditionsLunqualifiedId;
	vector<bool> conditionsRunqualifiedId;
	vector<int> startLines;
	//vector<int> endLines;

public:
	If();
	If(Node* compRtPtr, Node* trRtPtr);
	void setVariables();
	string getComponent();
	void checkComponent();
};

If::If() : Component()
{
	setStatementType(1);
}

If::If(Node* compRtPtr, Node* trRtPtr) : Component(compRtPtr, trRtPtr) {
	setStatementType(1);
	elses = 0;
}

void If::setVariables()
{
	Node* statementStart = getComponentRootNode();
	int numOfComponentChildren = getComponentRootNode()->getChildCount();
	Node* walker;
	Node* walker2;

	//find first line number
	walker = getComponentRootNode()->getChild(0);
	startLines.push_back(walker->getLineNum());

	//find the condition
	walker = nullptr;
	for (int i = 0; i < numOfComponentChildren; i++) {
		if (getComponentRootNode()->getChild(i)->getData() == "condition") {
			walker = getComponentRootNode()->getChild(i);
			break;
		}
	}

	if (walker == nullptr)
		conditions = "No condition was found";
	else
	{
		while (walker->getChildCount() == 1)
			walker = walker->getChild(0);
		
		// single condition, boolean value being tested
		if (walker->getChildCount() == 0) {
			if (walker->getParent()->getData() == "unqualifiedId") {
				conditionsLunqualifiedId.push_back(true);
				conditionsRunqualifiedId.push_back(false);
			}
			else {
				conditionsLunqualifiedId.push_back(false);
				conditionsRunqualifiedId.push_back(false);
			}
			conditionsL = walker->getData();
			conditions = "boolean";
		}
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
			conditionsL = walker2->getData();

			//get comparison operator
			conditions = walker->getChild(1)->getData();

			//get right side of condition
			walker2 = walker->getChild(2);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionsRunqualifiedId.push_back(true);
			else
				conditionsRunqualifiedId.push_back(false);
			conditionsR = walker2->getData();
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
			conditionsL = walker2->getData();

			//get comparison operator
			conditions = walker->getChild(1)->getData();
			conditions += walker->getChild(2)->getData();

			//get right side of condition
			walker2 = walker->getChild(2);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionsRunqualifiedId.push_back(true);
			else
				conditionsRunqualifiedId.push_back(false);
			conditionsR = walker2->getData();
		}
	}

	while (statementStart->getChildCount() == 7)	//7 children means there's an else
	{
		elses++;

		// find else
		walker = nullptr;
		for (int i = 0; i < numOfComponentChildren; i++) {
			if (getComponentRootNode()->getChild(i)->getData() == "else") {
				walker = getComponentRootNode()->getChild(i);
				break;
			}
		}
		if (walker != nullptr) {
			startLines.push_back(walker->getLineNum());

			statementStart = statementStart->getChild(6);
			if (statementStart->getChild(0)->getData() == "selectionStatement")	//means there's another if
			{
				statementStart = statementStart->getChild(0);
			}
		}
		else {
			break;
		}
	}
}


string If::getComponent() {
	string componentString = "<strong><u>If Statement</u></strong><br/>";

	if (startLines.size() > 1) {
		componentString += "Begins on line " + to_string(startLines[0]) + "<br/>";
		componentString += "Else begins on line " + to_string(startLines[1]) + "<br/>";
	}
	else {
		componentString += "<br/>If statement begins on line " + to_string(startLines[0]) + "<br/>";
	}
	componentString += "Number of elses: " + to_string(elses) + "<br/>";

	componentString += "Condition for If Statement: " + conditionsL + " " + conditions + " " + conditionsR + "<br/>";

	return componentString;
}

void If::checkComponent() {
	for (int i = 0; i < conditionsLunqualifiedId.size(); i++) {
		if (conditionsLunqualifiedId[i] == false && conditionsRunqualifiedId[i] == false) {
			setCorrectComponent(false);
			setCodeSmell("If statement on line" + to_string(startLines[0])+ " uses literals. Execution will always follow the same path.<br/>");
		}
	}
}

#endif