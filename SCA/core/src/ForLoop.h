#ifndef FOR_LOOP
#define FOR_LOOP

#include "SCA.h"
#include "Component.h"

using namespace std;

class ForLoop : public Component 
{
private:
	string iteratorInt;
	string iteratorIntAssignment;
	bool IntAssignmentUnqualifiedId;
	string condition;
	string conditionL;
	bool conditionLunqualifiedId;
	string conditionR;
	bool conditionRunqualifiedId;
	vector<string> expression;
	bool hasExpression;

public:
	ForLoop();
	ForLoop(Node* stOfStatement, Node* rtOfTree);

	void setVariables();

	void _extractExpression(Node* rt);
	string getComponent();
	void checkComponent();

	// simple checks to validate that loop will run and loop will terminate
	void _validateLoop(double initVal, double condVal, double stepVal, string stepIncOrDec);
};

ForLoop::ForLoop() : Component()
{
	setStatementType(0);
	condition = "";
	conditionLunqualifiedId = false;
	conditionRunqualifiedId = false;
	IntAssignmentUnqualifiedId = false;
}

ForLoop::ForLoop(Node* stOfStatement, Node* rtOfTree) : Component(stOfStatement, rtOfTree) {
	setStatementType(0);
	condition = "";
	iteratorInt = "";
	iteratorIntAssignment = "";
	conditionL = "";
	conditionR = "";
	conditionLunqualifiedId = false;
	conditionRunqualifiedId = false;
	IntAssignmentUnqualifiedId = false;
	hasExpression = false;
}

void ForLoop::setVariables()
{
	Node* walker;
	Node* walker2;

	// get component root node and number of children for use later in function
	walker = getComponentRootNode();
	int numOfComponentChildren = getComponentRootNode()->getChildCount();

	//find first line number
	addAnStartLine(walker->getChild(0)->getLineNum());
	//set last line number
	addAnEndLine(walker);

	//Find out what the iterator is called
	if (findNode("forInitStatement", getComponentRootNode())) {
		walker = getFoundNode();
		if (findNode("unqualifiedId", walker))
			walker2 = getFoundNode();
		else
			walker2 = nullptr;
	}
	else {
		walker = nullptr;
		walker2 = nullptr;
	}

	if (walker2 == nullptr)//unqualifiedId not found
		iteratorInt = "Not an unqualifiedId";
	else
	{
		iteratorInt = walker2->getChild(0)->getData();

		//find what value is assigned to the iterator
		if (findNode("literal", walker))
			walker2 = getFoundNode();
		else
			walker2 = nullptr;

		if (walker2 != nullptr)
			iteratorIntAssignment = walker2->getChild(0)->getData();
		else//check if assignment is unqualified id
		{
			if (findNode("initDeclarator", walker))
				walker = getFoundNode();
			else
				walker = nullptr;

			if (walker != nullptr) {
				walker2 = walker->getChild(1);
				if (findNode("unqualifiedId", walker2)) {
					walker2 = getFoundNode();
					iteratorIntAssignment = walker2->getChild(0)->getData();
				}
				else {
					walker2 = nullptr;
					iteratorIntAssignment = "No assignment made";
				}
			}
		}
	}

	//find the condition
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
		while (walker->getChildCount() == 1)
			walker = walker->getChild(0);

		if (walker->getChildCount() == 3)
		{
			//get left side of condtion
			walker2 = walker->getChild(0);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionLunqualifiedId = true;
			conditionL = walker2->getData();

			//get comparison operator
			condition = walker->getChild(1)->getData();

			//get right side of condition
			walker2 = walker->getChild(2);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionRunqualifiedId = true;
			conditionR = walker2->getData();
		}
		else if (walker->getChildCount() == 4)
		{
			//get left side of condtion
			walker2 = walker->getChild(0);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionLunqualifiedId = true;
			conditionL = walker2->getData();

			//get comparison operator
			condition = walker->getChild(1)->getData();
			condition += walker->getChild(2)->getData();

			//get right side of condition
			walker2 = walker->getChild(3);

			while (walker2->getChildCount() == 1)
				walker2 = walker2->getChild(0);

			if (walker2->getParent()->getData() == "unqualifiedId")
				conditionRunqualifiedId = true;
			conditionR = walker2->getData();
		}
	}
	
	//find the expression
	walker = nullptr;
	for (int i = 0; i < numOfComponentChildren; i++) {
		if (getComponentRootNode()->getChild(i)->getData() == "expression") {
			walker = getComponentRootNode()->getChild(i);
			break;
		}
	}

	if (walker == nullptr) 
		expression.push_back("No expression");
	else {
		_extractExpression(walker);	
		hasExpression = true;
	}
}

void ForLoop::_extractExpression(Node* rt) {
	Node* nodeIter = rt;
	if (nodeIter->getChildCount() == 0) {
		expression.push_back(nodeIter->getData());
		return;
	}
	int totalChildren = nodeIter->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		_extractExpression(nodeIter->getChild(i));
	}
}


string ForLoop::getComponent()
{
	int currLineNum = 0;
	int prevLineNum = 0;
	string htmlString = "";
	vector<int> startLineVec = getStartLine();
	vector<int> endLineVec = getEndLine();

	htmlString = "<strong><u>For Loop</u></strong><br/>";
	htmlString += "Begins on line " + to_string(startLineVec[0]) + "<br/>";

	htmlString += "Iterator: " + iteratorInt + "<br/>";
	htmlString += "Assignment to that iterator: " + iteratorIntAssignment + "<br/>";

	htmlString += "Stop Condition: " + conditionL + " " + condition + " " + conditionR + "<br/>";

	htmlString += "Step statement: ";
	for (string s : expression) 
		htmlString += s + " ";
	htmlString += "<br/>";

	//TO:DO: Fix this to get the correct end line
		//htmlString += "Ends on line " + to_string(endLineVec[0]) + "<br/>";

	return htmlString;
}

void ForLoop::checkComponent() {
	// make sure a variable is used to initialize for loop iteration
	if (iteratorInt == "Not an unqualifiedId") {
		setCorrectComponent(false);
		setCodeSmell("Use a variable to initialize For Loop.<br/>");
	}

	if (condition == "No condition was found") {
		setCorrectComponent(false);
		setCodeSmell("For Loop must contain a condition.<br/>");
	}

	// Only check init variable, condition and expression if they have been extracted
	if (getCorrectComponent() && hasExpression) {
		double initValDbl = 0.0;
		double condValDbl = 0.0;
		double stepValDbl = 0.0;
		string stepIncreaseOrDecrease = "";

		// Extract loop conditions into integer data types to perform testing on loop to check
		// if loop will run 0 times or if it's an infinite loop
		if (isNumeric(iteratorIntAssignment)) {
			initValDbl = stod(iteratorIntAssignment);
			if (isNumeric(conditionR)) 
				condValDbl = stod(conditionR);
			else if (isNumeric(conditionL))
				condValDbl = stod(conditionL);
			else
				goto postExtraction;
			
			for (int i = 0; i < expression.size(); i++) {
				if (isNumeric(expression[i])) 
					stepValDbl = stod(expression[i]);
				else if (expression[i] == "+" || expression[i] == "+=" || expression[i] == "++")
					stepIncreaseOrDecrease = "inc";
				else if (expression[i] == "-" || expression[i] == "-=" || expression[i] == "--")
					stepIncreaseOrDecrease = "dec";

			}
			_validateLoop(initValDbl, condValDbl, stepValDbl, stepIncreaseOrDecrease);
		}
		else {
			goto postExtraction;
		}
	}

	postExtraction:
	return;
}

void ForLoop::_validateLoop(double initVal, double condVal, double stepVal, string stepIncOrDec) {
	if (condition == "<") {
		if (initVal >= condVal) {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may run zero times.<br/>");
		}
		if (initVal < condVal && stepIncOrDec == "dec") {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may never terminate.<br/>");
		}
	}
	else if (condition == ">") {
		if (initVal <= condVal) {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may run zero times.<br/>");
		}
		if (initVal > condVal && stepIncOrDec == "inc") {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may never terminate.<br/>");
		}
	}
	else if (condition == "<=") {
		if (initVal > condVal) {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may run zero times.<br/>");
		}
		if (initVal <= condVal && stepIncOrDec == "dec") {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may never terminate.<br/>");
		}
	}
	else if (condition == ">=") {
		if (initVal < condVal) {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may run zero times.<br/>");
		}
		if (initVal >= condVal && stepIncOrDec == "inc") {
			setCorrectComponent(false);
			setCodeSmell("Check For Loop, it may never terminate.<br/>");
		}
	}
}


#endif // !FOR_LOOP