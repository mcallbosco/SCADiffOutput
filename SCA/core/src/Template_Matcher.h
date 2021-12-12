#ifndef TEMPLATE_MATCHER
#define TEMPLATE_MATCHER

#include "SCA.h"
#include "While_Loop.h"
#include "ForLoop.h"
#include "Switch.h"
#include "If.h"
#include <string>

using namespace std;

class Template_Matcher {
private:
	unordered_map <int, Rule> template_table;
	int rulesViolatedEntries;
	int rulesToCheckEntries;
	int* rulesToCheck;
	int* rulesViolated;
	int* rulesViolatedLines;
	string suggestions;

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

	//Checks for errors in all components.
	void checkAllComponents(vector<ForLoop*> forLoopComponents, vector<While_Loop*> whileLoopComponents, vector<If*> ifComponents, vector<Switch*> switchComponents);
};

Template_Matcher::Template_Matcher() 
{
	rulesToCheckEntries = 0;
	rulesViolatedEntries = 0;
	suggestions = "";
}

void Template_Matcher::searchFirstToken(string token)
{
	rulesToCheckEntries = 0;

	int dataLength = token.length();//get length of give token

	//for each rule in the table
	for (int i = 0; i < template_table.size(); i++)
	{
		//get first token from rule i using dataLength
		string firstToken = template_table[i].getRule().substr(0, dataLength);
		if (firstToken == token)//if match then add to rulesToCheck[]
		{
			rulesToCheck[rulesToCheckEntries] = template_table[i].getRuleNum();
			rulesToCheckEntries++;
		}
	}
}

void Template_Matcher::checkTreeForErrors(Node* rt)
{
	searchFirstToken(rt->getData());
	for (int i = 0; i < rulesToCheckEntries; i++)//for each rule that needs to be checked for this node, search for full violation on those rules.
		checkFullViolation(rt, template_table[rulesToCheck[i]]);

	int totalChildren = rt->getChildCount();

	//continue looking for error in children nodes
	for (int i = 0; i < totalChildren; i++)
		checkTreeForErrors(rt->getChild(i));
}

bool Template_Matcher::checkFullViolation(Node* rt, Rule currRule)
{
	//get just the rule from the Rule class
	string templateRule = currRule.getRule();

	//removes white space in rule
	for (int i = 0; i < templateRule.length(); i++)
	{
		if (isspace(templateRule[i]))
			templateRule.erase(i, 1);
	}

	Tree tempTree;
	//get full rule from tree based off number of tokens in a currRule
	string treeRule = tempTree.getNextNtokens(rt, currRule.getTokens());

	if (treeRule == templateRule)
	{
		bool alreadyThere = false;
		for (int i = 0; i < rulesViolatedEntries; i++)
		{
			//check if rule is already in rulesViolated[], check if ruleNum and lineNum are the same as anything in rulesViolated[]
			if ((rulesViolated[i] == currRule.getRuleNum()) && (rulesViolatedLines[i] == rt->getLineNum()))
				alreadyThere = true;

			if (alreadyThere)
				break;
		}
		if (!alreadyThere)//if not in rulesViolated[], add it
		{
			rulesViolated[rulesViolatedEntries] = currRule.getRuleNum();
			rulesViolatedLines[rulesViolatedEntries] = rt->getLineNum();
			rulesViolatedEntries++;
			return true;
		}
	}
	return false;//no violation found
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
	if (rulesViolatedEntries != 0)
	{
		for (int i = 0; i < rulesViolatedEntries; i++)
		{
			suggestions += "Line " + rulesViolatedLines[i];
			suggestions += ": " + template_table[rulesViolated[i]].getSuggestion() + "\n";
		}
	}

	return suggestions;
}


void Template_Matcher::checkAllComponents(vector<ForLoop*> forLoopComponents, vector<While_Loop*> whileLoopComponents, vector<If*> ifComponents, vector<Switch*> switchComponents)
{
	for (int i = 0; i < whileLoopComponents.size(); i++)
	{
		//check for errors
		//if (errors)
		//whileLoopComponents[i].setCorrect(false)
		//else
		//whileLoopComponents[i].setCorrect(true)
	}

	for (int i = 0; i < forLoopComponents.size(); i++)
	{
		//check for errors
		//if (errors)
		//forLoopComponents[i].setCorrect(false)
		//else
		//forLoopComponents[i].setCorrect(true)
	}

	for (int i = 0; i < switchComponents.size(); i++)
	{
		//check for errors
		//if (errors)
		//switchComponents[i].setCorrect(false)
		//else
		//switchComponents[i].setCorrect(true)
	}

	for (int i = 0; i < ifComponents.size(); i++)
	{
		//check for errors
		//if (errors)
		//ifComponents[i].setCorrect(false)
		//else
		//ifComponents[i].setCorrect(true)
	}
}

#endif // !TEMPLATE_MATCHER