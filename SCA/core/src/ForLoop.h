#ifndef FOR_LOOP
#define FOR_LOOP

#include "SCA.h"
#include "Component.h"

using namespace std;

class ForLoop : Component 
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
	Component componentClass;
	string expression;
	string plusplusOrminusminus;

public:
	ForLoop();

	void setVariables(Tree* rt);

	
};

ForLoop::ForLoop()
{
	componentClass.setStatementType(0);
	condition = "";
	conditionLunqualifiedId = false;
	conditionRunqualifiedId = false;
	IntAssignmentUnqualifiedId = false;
}

void ForLoop::setVariables(Tree* rt)
{
	componentClass.setStartOfStatement(rt);
	Node* statementStart = componentClass.getStartOfStatement();
	Node* walker;
	Node* walker2;

	//find first line number
	walker = findNode("for", statementStart);
	addAnStartLine(walker->getLineNum());

	//set last line number
	addAnEndLine(statementStart);

	//Find out what the iterator is called
	walker = findNode("forInitStatement", statementStart);
	walker2 = findNode("unqualifiedId", walker);

	if (walker2 == nullptr)//unqualifiedId not found
		iteratorInt = "Not an unqualifiedId";
	else
	{
		iteratorInt = walker2->getChild(0)->getData();

		//find what value is assigned to the iterator
		walker2 = walker->getChild(1);
		walker = findNode("literal", walker2);

		if (walker != nullptr)
			iteratorIntAssignment = walker->getChild(0)->getData();
		else//check if assignment is unqualified id
		{
			walker = findNode("unqualifiedId", walker2);
			if (walker != nullptr)
			{
				iteratorIntAssignment = walker->getChild(0)->getData();
				IntAssignmentUnqualifiedId = true;
			}
			else//no assigment for iterator
				iteratorIntAssignment = "No assignment made";
		}
	}

	//find the condition
	walker = findNode("condition", statementStart);
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
	walker = findNode("expression", statementStart);
	while (walker->getChildCount() == 1)
		walker = walker->getChild(0);

	plusplusOrminusminus = walker->getChild(1)->getData();

	walker = walker->getChild(0);
	while (walker->getChildCount() == 1)
		walker = walker->getChild(0);

	expression = walker->getData();
}

#endif // !FOR_LOOP