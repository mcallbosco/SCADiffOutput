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
	string condition;
	bool conditionIsUnqualifiedId;
	bool conditionIsLiteral;
	int startLine;
	vector<string> cases;
	vector<int> caseLines;
	bool hasDefault;
	string iteratorInt;
	int defaultLine;


public:
	Switch();
	Switch(Node* compRtPtr, Node* trRtPtr);

	void setVariables();

	void _extractCase(Node* rt);

	string getComponent();
	void checkComponent();
};

Switch::Switch() : Component()
{
	setStatementType(1);
	condition = "";
	conditionIsUnqualifiedId = false;
	numOfCases = 0;
	hasDefault = false;
}

Switch::Switch(Node* compRtPtr, Node* trRtPtr) : Component(compRtPtr, trRtPtr) {
	setStatementType(1);
	numOfCases = 0;
	numOfBreaks = 0;
	startLine = 0;
	defaultLine = 0;
	conditionIsUnqualifiedId = false;
	hasDefault = false;
	conditionIsLiteral = false;
	iteratorInt = "";
	condition = "";
}

void Switch::setVariables()
{
	Node* statementStart = getComponentRootNode();
	int numOfComponentChildren = getComponentRootNode()->getChildCount();
	Node* walker = getComponentRootNode();
	Node* walker2, *walker3, *walker4;

	//find first line number
	walker = walker->getChild(0);
	startLine = walker->getLineNum();

	//find condition
	walker = nullptr;
	for (int i = 0; i < numOfComponentChildren; i++) {
		if (getComponentRootNode()->getChild(i)->getData() == "condition") {
			walker = getComponentRootNode()->getChild(i);
			break;
		}
	}

	if (walker == nullptr)
		condition = "No condition was found";
	else
	{
		while (walker->getChildCount() != 0) {
			walker = walker->getChild(0);
		}

		if (walker->getParent()->getData() == "unqualifiedId") {
			conditionIsUnqualifiedId = true;
			condition = walker->getData();
		}
		else if (walker->getParent()->getData() == "literal") {
			conditionIsLiteral = true;
			condition = walker->getData();
		}
		else {
			condition = walker->getData();
		}
	}

	//find cases
	walker = nullptr;
	for (int i = 0; i < numOfComponentChildren; i++) {
		if (getComponentRootNode()->getChild(i)->getData() == "statement") {
			walker = getComponentRootNode()->getChild(i);
			break;
		}
	}

	if (walker == nullptr) {
		cases.push_back("Could not find case statements");
	}
	else {

		if (findNode("statementSeq", walker)) {
			walker = getFoundNode();

			for (int i = 0; i < walker->getChildCount(); i++)
			{
				walker2 = walker->getChild(i);

				if (walker2->getChild(0)->getData() == "labeledStatement")
				{
					walker2 = walker2->getChild(0);
					walker3 = walker2->getChild(0);
					walker4 = walker2->getChild(1);
					
					if (walker3->getData() == "case")
					{
						numOfCases++;
						caseLines.push_back(walker3->getLineNum());
						_extractCase(walker4);
					}
					else if (walker3->getData() == "default")
					{
						hasDefault = true;
						defaultLine = walker3->getLineNum();
					}
				}
				else if (walker2->getChild(0)->getData() == "jumpStatement")
				{
					numOfBreaks++;
				}
			}
		}
		else {
			cases.push_back("Unable to find case");
		}
		
		
	}
}

void Switch::_extractCase(Node* rt) {
	Node* iterNode = rt;
	if (iterNode->getChildCount() == 0) {
		cases.push_back(iterNode->getData());
		return;
	}
	int numOfChildren = iterNode->getChildCount();
	for (int i = 0; i < numOfChildren; i++) {
		_extractCase(iterNode->getChild(i));
	}
}

string Switch::getComponent() {
	string componentString = "";

	componentString = "<strong><u>Switch Structure</u></strong><br/>";
	componentString += "Begins on line " + to_string(startLine) + "<br/>";
	componentString += "Condition being tested: " + condition + "<br/>";
	componentString += "Number of cases: " + to_string(numOfCases) + "<br/>";
	componentString += "Number of breaks: " + to_string(numOfBreaks) + "<br/>";
	componentString += "Contains default case: ";
	if (hasDefault)
		componentString += "Yes, on line " + to_string(defaultLine) + "<br/>";
	else 
		componentString += "No<br/>";
	
	componentString += "List of Cases:<br/>";
	for (int i = 0; i < caseLines.size(); i++) {
		componentString += "case (" + cases[i] + ") on line " + to_string(caseLines[i]) + "<br/>";
	}

	return componentString;

}

void Switch::checkComponent() {
	// make sure same # of breaks as there are cases
	// default doesn't count as a case so if there is a default there should be (cases + 1) breaks
	// if no default add string to code smells and also make sure breaks == cases
	if (hasDefault) {
		// + 1 to account for the default case
		if ((numOfCases + 1) != numOfCases) {
			setCorrectComponent(false);
			setCodeSmell("Switch statement on line " + to_string(startLine)+ ": " + "Make sure all cases contain a break statement, including the default case.<br/>");
		}
	}
	else {
		setCorrectComponent(false);
		setCodeSmell("Switch statement on line " + to_string(startLine)+ ": " + "Be sure to include a default case.<br/>");
	}

	// if number of cases is 1, suggest using an if/else statment
	if (numOfCases == 1) {
		setCorrectComponent(false);
		setCodeSmell("Switch statement on line " + to_string(startLine)+ ": " + "You may want to use an if-else statement instead of a switch structure.<br/>");
	}

	// check to make sure condtion is testing a variable and not a literal
	if (conditionIsLiteral) {
		setCorrectComponent(false);
		setCodeSmell("Switch statement on line " + to_string(startLine)+ ": " + "Make sure your switch structure is testing a variable.<br/>");
	}

	if (condition == "No condition was found") {
		setCorrectComponent(false);
		setCodeSmell("Switch statement on line " + to_string(startLine)+ ": " + "Could not find a condition for Switch structure.<br/>");
	}

	if (cases.size() > 0 && cases[0] == "Could not find case statements") {
		setCorrectComponent(false);
		setCodeSmell("Switch statement on line " + to_string(startLine)+ ": " + "Make sure your Switch structure has cases.<br/>");
	}
}

#endif