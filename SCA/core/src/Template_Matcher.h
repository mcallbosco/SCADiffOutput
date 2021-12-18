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
	TemplateTable* rules;
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

	//makes ruleesViolated[] and rulesToCheck[] size equal to template_table.size().
	void setRulesArraySize();

	//passes rulesViolated by reference to another array;
	void getRulesViolated(int* rules_violated);

	//read rules into memory
	bool readRulesIntoMemory(const string& pathToRules);

	//returns the rules violated using templatetable.getSuggestion() for each rule in ruleViolated[].
	string outputSuggestions();

	//returns template table size, the number of rules.
	int getTemplateTableSize();

};

Template_Matcher::Template_Matcher() 
{
	rulesToCheckEntries = 0;
	rulesViolatedEntries = 0;
	suggestions = "";
	rules = new TemplateTable();

}

bool Template_Matcher::readRulesIntoMemory(const string& pathToRules) {
	if (rules->loadTemplateTable(pathToRules))
		return true;
	else
		return false;
}

void Template_Matcher::searchFirstToken(string token)
{
	rulesToCheckEntries = 0;

	int dataLength = token.length();//get length of give token

	//for each rule in the table
	for (int i = 0; i < rules->getTemplateTable().size(); i++)
	{
		//get first token from rule i using dataLength
		string firstToken = rules->getTemplateTable()[i].getRule().substr(0, dataLength);
		if (firstToken == token)//if match then add to rulesToCheck[]
		{
			rulesToCheck[rulesToCheckEntries] = rules->getTemplateTable()[i].getRuleNum();
			rulesToCheckEntries++;
		}
	}
}

void Template_Matcher::checkTreeForErrors(Node* rt)
{
	searchFirstToken(rt->getData());
	for (int i = 0; i < rulesToCheckEntries; i++)//for each rule that needs to be checked for this node, search for full violation on those rules.
		checkFullViolation(rt, rules->getTemplateTable()[rulesToCheck[i]]);

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
			for (int i = 0; i < rules->getTemplateTable().size(); i++) {
				if ((rulesViolated[i] == currRule.getRuleNum()) && (rulesViolatedLines[i] == rt->getLineNum())) {
					alreadyThere = true;
					break;
				}
			}
		}
		if (!alreadyThere)//if not in rulesViolated[], add it
		{
			rulesViolated[rulesViolatedEntries] = currRule.getRuleNum();

			// move to leaf node to obtain the line number
			Node* temp = rt;
			while (temp->getChildCount() != 0) {
				temp = temp->getChild(0);
			}

			rulesViolatedLines[rulesViolatedEntries] = temp->getLineNum();
			rulesViolatedEntries++;
			return true;
		}
	}
	return false;//no violation found
}

void Template_Matcher::setRulesArraySize() {
	rulesViolated = new int [rules->getTemplateTable().size()];
	rulesToCheck = new int [rules->getTemplateTable().size()];
	rulesViolatedLines = new int[rules->getTemplateTable().size()];
	rulesToCheckEntries = 0;
	rulesViolatedEntries = 0;
}

void Template_Matcher::getRulesViolated(int* rules_violated)
{
	for (int i = 0; i < rules->getTemplateTable().size(); i++)
		rules_violated[i] = rulesViolated[i];
}

int Template_Matcher::getTemplateTableSize()
{
	return rules->getTemplateTable().size();
}

string Template_Matcher::outputSuggestions()
{
	if (rulesViolatedEntries != 0)
	{
		for (int i = 0; i < rulesViolatedEntries; i++)
		{
			suggestions += "<strong>Line " + to_string(rulesViolatedLines[i]) + ":</strong> " + rules->getTemplateTable()[rulesViolated[i]].getSuggestion() + "\n";
		}
	}

	return suggestions;
}



#endif // !TEMPLATE_MATCHER