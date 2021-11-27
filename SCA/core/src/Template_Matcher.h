#ifndef TEMPLATE_MATCHER
#define TEMPLATE_MATCHER

#include "SCA.h"
#include "While_Loop.h"
#include <string>

using namespace std;

class Template_Matcher {
private:
	unordered_map <int, Rule> template_table;
	string iterationStmt = "iterationStatement";
	string selectionStmt = "selectionStatement";
	int rulesViolatedEntries;
	int rulesToCheckEntries;
	int* rulesToCheck;
	int* rulesViolated;
	int* rulesViolatedLines;
	string suggestions;
	string componentString;
	vector<Node*> iterationNodes;
	vector<Node*> selectionNodes;
	vector<While_Loop*> whileLoopComponents;

public:
	//constructor sets entries to 0 and suggestions to "".
	Template_Matcher();

	//compares passed token to all the first tokens in rules in the template table, adds ruleNum that matched to rulesToCheck[].
	void searchFirstToken(string token);

	//uses searchFirstToken and checkFullViolation to find errors. Puts the number of the rules violated into rulesViolated[].
	void checkTreeForErrors(Node* rt);

	//starts from rt and travels the tree to compare to rule. If true violation commited, add to rulesViolated[].
	bool checkFullViolation(Node* rt, Rule currRule);

	//sets template table, ideally getting table from TemplateTable.h
	void setTemplateTable(unordered_map <int, Rule> templatetable);

	//makes ruleesViolated[] and rulesToCheck[] size equal to template_table.size().
	void setRulesArraySize();

	//passes rulesViolated by reference to another array;
	void getRulesViolated(int* rules_violated);

	//returns the rules violated using templatetable.getSuggestion() for each rule in ruleViolated[].
	string outputSuggestions();

	//returns template table size, the number of rules.
	int getTemplateTableSize();

	// fills the iteration node vector with all node instances of iteration statements in source code
	void _fillIterationNodeVector(Tree* tree);

	// fills the selection node vector with all node instances of selection statements in source code
	void _fillSelectionNodeVector(Tree* tree);

	// Calls the fill node vector functions then iterates through each vector handing node to corresponding component class to build component
	void retreiveComponents(Tree* tree);

};

Template_Matcher::Template_Matcher() 
{
	rulesToCheckEntries = 0;
	rulesViolatedEntries = 0;
	suggestions = "";
	componentString = "";
}

void Template_Matcher::searchFirstToken(string token)
{
	rulesToCheckEntries = 0;

	int dataLength = token.length();

	//for each rule in the table
	for (int i = 0; i < template_table.size(); i++)
	{
		//get first token from rule i
		string firstToken = template_table[i].getRule().substr(0, dataLength);
		if (firstToken == token)
		{
			rulesToCheck[rulesToCheckEntries] = template_table[i].getRuleNum();
			rulesToCheckEntries++;
		}
	}
}

void Template_Matcher::checkTreeForErrors(Node* rt)
{
	searchFirstToken(rt->getData());
	for (int i = 0; i < rulesToCheckEntries; i++)//for each rule that needs to be checked, search for full violation on those rules.
		checkFullViolation(rt, template_table[rulesToCheck[i]]);

	int totalChildren = rt->getChildCount();

	//continue looking for error in children nodes
	for (int i = 0; i < totalChildren; i++)
		checkTreeForErrors(rt->getChild(i));
}

bool Template_Matcher::checkFullViolation(Node* rt, Rule currRule)
{
	int dataLength = rt->getData().length();
	string templateRule = currRule.getRule();

	//first token from rule
	string ruleFirstToken = templateRule.substr(0, dataLength);

	//removes white space in rule
	for (int i = 0; i < templateRule.length(); i++)
	{
		if (isspace(templateRule[i]))
			templateRule.erase(i, 1);
	}

	Tree tempTree;
	string treeRule = tempTree.getNextNtokens(rt, currRule.getTokens());

	if (treeRule == templateRule)
	{
		bool alreadyThere = false;
		for (int i = 0; i < template_table.size(); i++)
		{
			if ((rulesViolated[i] == currRule.getRuleNum()) && (rulesViolatedLines[i] == rt->getLineNum()))
				alreadyThere = true;

			if (alreadyThere)
				break;
		}
		if (!alreadyThere)
		{
			rulesViolated[rulesViolatedEntries] = currRule.getRuleNum();
			rulesViolatedLines[rulesViolatedEntries] = rt->getLineNum();
			rulesViolatedEntries++;
			return true;
		}
	}
	return false;
}

void Template_Matcher::setTemplateTable(unordered_map <int, Rule> templatetable) {
	template_table = templatetable;
}

void Template_Matcher::setRulesArraySize() {
	rulesViolated = new int [template_table.size()];
	rulesToCheck = new int [template_table.size()];
	rulesViolatedLines = new int[template_table.size()];
	rulesToCheckEntries = 0;
	rulesViolatedEntries = 0;
}

void Template_Matcher::getRulesViolated(int* rules_violated)
{
	for (int i = 0; i < template_table.size(); i++)
		rules_violated[i] = rulesViolated[i];
}

int Template_Matcher::getTemplateTableSize()
{
	return template_table.size();
}

string Template_Matcher::outputSuggestions()
{
	for (int i = 0; i < rulesViolatedEntries; i++)
	{
		if (rulesViolated[i] == -1)
			break;
		suggestions += "Line " + rulesViolatedLines[i];
		suggestions += ": " + template_table[rulesViolated[i]].getSuggestion() + "\n";
	}

	return suggestions;
}

void Template_Matcher::_fillIterationNodeVector(Tree* tree) {
	tree->filltokenNodeVector(iterationStmt, tree->getRoot());
	iterationNodes = tree->getTokenNodes();
	tree->clearTokenNodes();
}

	
void Template_Matcher::_fillSelectionNodeVector(Tree* tree) {
	tree->filltokenNodeVector(selectionStmt, tree->getRoot());
	selectionNodes = tree->getTokenNodes();
	tree->clearTokenNodes();
}

void Template_Matcher::retreiveComponents(Tree* tree) {
	// populate vectors with nodes from tree
	_fillIterationNodeVector(tree);
	_fillSelectionNodeVector(tree);
	Node* temp;
	string testToken = "";

	// iterate though each node that contains an iterationStatement
	for (int i = 0; i < iterationNodes.size(); i++) {
		temp = iterationNodes[i];
		while (temp->getChildCount() != 0) {
			temp = temp->getChild(0);
		}
		testToken = temp->getData();
		
		if (testToken == "for") {
			// hand control to for_component, pass in rootNode of Tree and iterationNode[i]
			break;
		}
		else if (testToken == "while") {
			While_Loop* aWhileLoop = new While_Loop(iterationNodes[i], tree->getRoot(), "while");
			aWhileLoop->extractAllInfo();
			whileLoopComponents.push_back(aWhileLoop);
			break;
		}
		else if (testToken == "do") {
			While_Loop* aDoWhileLoop = new While_Loop(iterationNodes[i], tree->getRoot(), "do");
			aDoWhileLoop->extractAllInfo();
			whileLoopComponents.push_back(aDoWhileLoop);
			break;
		}
	}

	// iterate through each node that contains a selectionStatement
	for (int i = 0; i < selectionNodes.size(); i++) {
		temp = iterationNodes[i];
		while (temp->getChildCount() != 0) {
			temp = temp->getChild(0);
		}

		testToken = temp->getData();
		if (testToken == "if") {
			// hand control to if_component, pass in rootNode of Tree and iterationNode[i]
			break;
		}
		else if (testToken == "switch") {
			// hand control to switch_component, pass in rootNode of Tree and iterationNode[i]
			break;
		}
	}

	for (int i = 0; i < whileLoopComponents.size(); i++) {
		componentString += whileLoopComponents[i]->getComponent();
	}

	// add component string to end of suggestions string to be used by createHTML function
	suggestions = suggestions + componentString;
}



#endif // !TEMPLATE_MATCHER