#ifndef WHILE_COMPONENT_
#define WHILE_COMPONENT_

#include "Component.h"
#include <algorithm>

class While_Loop : public Component 
{
private:
    string loopType;
    int begLineNum;
    vector<Node*> setupVariableInstances;
    vector<Node*> setupVariable;
    vector<string> testVariable;
    vector<Node*> testExpression;
    vector<Node*> body;
    vector<Node*> increment;
    vector<Node*> tempSetupVar;
    Node* conditionRtNode;
    Node* stmtRtNode;
public:
    While_Loop();
    While_Loop(Node* compRtPtr, Node* treeRtPtr, string lType);

    // uses member functions to extract all information from the tree
    void extractAllInfo();
    
    // extracts the line number where the loop begins
    void extractBegLineNum();

    // extracts test expression of while or do while loop
    void extractTestExpression();
    void _extractTestExpression(Node* condRtNode);

    // extracts the body of do...while or while statement
    void extractBody();
    void _extractBody(Node* stRtNode);

    // extracts the increment or decrement statment for test condition
    void extractIncrement();

    // extracts the setup variable from the source code used in test expression
    void extractSetupVariable();
    void _extractSetupVariable(Node* trRtNode);

    void printLoopInfo();
    string getComponent();

    void checkComponent();

    int getBegLineNum();
    string getLoopType();

};

While_Loop::While_Loop() {
    loopType = "";
    conditionRtNode = nullptr;
    stmtRtNode = nullptr;
    begLineNum = 0;
}

While_Loop::While_Loop(Node* compRtNode, Node* treeRtNode, string lType) : Component(compRtNode, treeRtNode) {
    loopType = lType;
    conditionRtNode = nullptr;
    stmtRtNode = nullptr;
    begLineNum = 0;
    setStatementType(0);
}

void While_Loop::extractBegLineNum() {
    if (getComponentRootNode()->getChildCount() > 0) {
        begLineNum = getComponentRootNode()->getChild(0)->getLineNum();
    }
}

void While_Loop::extractTestExpression() {
    Node* walker = getComponentRootNode();
    for (int i = 0; i < getComponentRootNode()->getChildCount(); i++) {
        if (loopType == "while" && walker->getChild(i)->getData() == "condition") {
            conditionRtNode = walker->getChild(i);
            break;
        }
        else if (loopType == "do" && walker->getChild(i)->getData() == "expression") {
            conditionRtNode = walker->getChild(i);
            break;
        }
    }
    if (conditionRtNode != nullptr) {
        _extractTestExpression(conditionRtNode);
    }
    else {
        cout << "Unable to locate condition expression in while loop" << endl;
    }
}

void While_Loop::_extractTestExpression(Node* condRtNode) {
    Node* iterNode = condRtNode;

    if (iterNode->getChildCount() == 0) {
		testExpression.push_back(iterNode);
		return;
	}

	int totalChildren = iterNode->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		_extractTestExpression(iterNode->getChild(i));
    }
}

void While_Loop::extractBody() {
    Node* walker = getComponentRootNode();
    for (int i = 0; i < getComponentRootNode()->getChildCount(); i++) {
        if (walker->getChild(i)->getData() == "statement") {
            stmtRtNode = walker->getChild(i);
            break;
        }
    }
    if (stmtRtNode != nullptr) {
        _extractBody(stmtRtNode);
    }
    else {
        cout << "Unable to locate body of while loop." << endl;
    }
}

void While_Loop::_extractBody(Node* stRtNode) {
    Node* iterNode = stRtNode;

    if (iterNode->getChildCount() == 0) {
        body.push_back(iterNode);
        return;
    }

    int totalChildren = iterNode->getChildCount();

    for (int i = 0; i < totalChildren; i++) {
        _extractBody(iterNode->getChild(i));
    }
}

void While_Loop::extractIncrement() {
    for (int i = 0; i < testExpression.size(); i++) {
        if (testExpression[i]->getParent()->getData() == "unqualifiedId") {
            testVariable.push_back(testExpression[i]->getData());
        }
    }
    // go through each leaf node in the body and check if the test variable is being updated
    for (int i = 0; i < body.size(); i++) {
        if (find(testVariable.begin(), testVariable.end(), body[i]->getData()) != testVariable.end()) {
            for (int j = i; j < body.size(); j++) {
                if (body[j]->getData() != ";") {
                    increment.push_back(body[j]);
                }
                else {
                    increment.push_back(body[j]);
                    i = j;
                    break;
                }
            }
        }
    }
}

void While_Loop::extractSetupVariable() {
    // extract all leaf nodes up the point where the loop begins
    _extractSetupVariable(getTreeRootNode());
    Node* markBegOfStatement = new Node("StatementBegin");

    // run through the nodes of source code prior to the beginning of the loop to find the setup variable
    for (int i = 0; i < tempSetupVar.size(); i++) {
        if (find(testVariable.begin(), testVariable.end(), tempSetupVar[i]->getData()) != testVariable.end()) {
            for (int j = i; j < tempSetupVar.size(); j++) {
                if (tempSetupVar[j]->getData() != ";") {
                    // if variable is found mark the beginning of the statement in vector of nodes.
                    if (j == i) {
                        setupVariableInstances.push_back(markBegOfStatement);
                    }
                    setupVariableInstances.push_back(tempSetupVar[j]);
                }
                else {
                    setupVariableInstances.push_back(tempSetupVar[j]);
                    i = j;
                    break;
                }
            }
        }
    }

    // extract last statment prior to the start of the for loop using the test variable
    for (int i = setupVariableInstances.size() - 1; i >= 0; i--) {
        if (setupVariableInstances[i]->getData() == "StatementBegin") {
            break;
        }
        else {
            setupVariable.push_back(setupVariableInstances[i]);
        }
    }

    // reverse order of the setup variable b/c it was extracted in reverse order
    reverse(setupVariable.begin(), setupVariable.end());


}

void While_Loop::_extractSetupVariable(Node* trRtNode) {
    Node* iterNode = trRtNode;
    if (iterNode->getChildCount() == 0) {
        if (iterNode->getLineNum() < begLineNum) {
            tempSetupVar.push_back(iterNode);
        }
    }
    
    int totalChildren = iterNode->getChildCount();

    for (int i = 0; i < totalChildren; i++) {
        _extractSetupVariable(iterNode->getChild(i));
    }
}

void While_Loop::extractAllInfo() {
    // extracts the line number where the loop begins
    extractBegLineNum();

    // extracts test expression of while or do while loop
    extractTestExpression();

    // extracts the body of do...while or while statement
    extractBody();

    // extracts the increment or decrement statment for test condition
    extractIncrement();

    // extracts the setup variable from the source code used in test expression
    extractSetupVariable();
}

void While_Loop::printLoopInfo() {
    int currLineNum = 0;
    int prevLineNum = 0;

    cout << "Loop Type: " << loopType << endl;

    cout << "Begins on Line " << begLineNum << endl;

    cout << "Setup Variable ";
    for (int i = 0; i < setupVariable.size(); i++) {
        if (i == 0) {
            cout << "on line " << setupVariable[i]->getLineNum() << ": ";
        }
        cout << setupVariable[i]->getData() << " ";
    }

    cout << endl << "Test Variable(s): ";
    for (int i = 0; i < testVariable.size(); i++) {
        cout << testVariable[i] << " ";
    }

    cout << endl << "Increment Statement(s): ";
    for (int i = 0; i < increment.size(); i++) {
        prevLineNum = currLineNum;
        currLineNum = increment[i]->getLineNum();
        if (prevLineNum != currLineNum) {
            cout << endl << "\t" << increment[i]->getData() << " ";
        }
        else {
            cout << increment[i]->getData() << " ";
        }
    }

    cout << endl << loopType << " (";
    for (int i = 0; i < testExpression.size(); i++) {
        cout << testExpression[i]->getData() << " ";
    }

    cout << ")";

    // reset current and previous line numbers
    currLineNum = 0;
    prevLineNum = 0;
    for (int i = 0; i < body.size(); i++) {
        prevLineNum = currLineNum;
        currLineNum = body[i]->getLineNum();
        if (prevLineNum != currLineNum) {
            cout << endl << body[i]->getData() << " ";
        }
        else {
            cout << body[i]->getData() << " ";
        }
    }
    cout << endl << endl;

}

int While_Loop::getBegLineNum() {
    return begLineNum;
}

string While_Loop::getLoopType() {
    return loopType;
}

string While_Loop::getComponent() {
    int currLineNum = 0;
    int prevLineNum = 0;
    string htmlString = "";

    if (loopType == "do")
        htmlString = "<strong><u>Do While Loop</u></strong><br/>";
    else
        htmlString = "<strong><u>While Loop</u></strong><br/>";

    htmlString += "<strong>Begins on Line " + to_string(begLineNum) + "</strong><br/>";

    htmlString += "<strong>Setup Variable ";
    for (int i = 0; i < setupVariable.size(); i++) {
        if (i == 0) {
            htmlString += "on line " + to_string(setupVariable[i]->getLineNum()) + ":</strong> ";
        }
        htmlString += setupVariable[i]->getData() + " ";
    }

    htmlString += "<br/><strong>Stop Condition:</strong> ";
    for (int i = 0; i < testExpression.size(); i++) {
        htmlString += testExpression[i]->getData() + " ";
    }

    htmlString += "<br/><strong>Step Statement(s):</strong> ";
    for (int i = 0; i < increment.size(); i++) {
        prevLineNum = currLineNum;
        currLineNum = increment[i]->getLineNum();
        if (prevLineNum != currLineNum) {
            htmlString += "<br/><strong>Line " + to_string(increment[i]->getLineNum()) + ":</strong> " + increment[i]->getData() + " ";
        }
        else {
            htmlString += increment[i]->getData() + " ";
        }
    }

    return htmlString;
}

void While_Loop::checkComponent() {
    // check if variable used in test condition is updated in body
    if (increment.size() == 0) {
        setCorrectComponent(false);
        setCodeSmell("Make sure you update your test variable in the body of the loop.<br/>");
    }

    // if boolean value is used as test expression, make sure it's not a literal and if it is
    // make sure there is a break somewhere to terminate loop execution
    if (testExpression.size() == 1) {
        if (testExpression[0]->getParent()->getData() == "literal") {
            if (!findNode("break", getComponentRootNode())) {
                setCorrectComponent(false);
                setCodeSmell("Make sure there is a break statement inside loop body to terminate execution.<br/>");
            }
        }
    }
    
    // check to make sure test expression doesn't compare literals
    if (testExpression.size() == 3) {
        if (testExpression[0]->getParent()->getData() == "literal" && testExpression[2]->getParent()->getData() == "literal") {
            if (!findNode("break", getComponentRootNode())) {
                setCorrectComponent(false);
                setCodeSmell("Be sure to use a variable in test expression, loop may never terminate.<br/>");
            }
            else {
                setCorrectComponent(false);
                setCodeSmell("Test expression compares two literals, is this necessary?<br/>");
            }
        }
    }
}

#endif // !WHILE_COMPONENT_