#ifndef RULE
#define RULE

#include <string>

using namespace std;

//Rule class to be stored in unordered_map, holds template table info
class Rule
{
private:
	int ruleNum;
	int tokens;
	string rule;
	string suggestion;
public:
	int getRuleNum() {
		return ruleNum;
	}

	void setRuleNum(int ruleNumber) {
		ruleNum = ruleNumber;
	}

	int getTokens() {
		return tokens;
	}

	void setTokens(int Tokens) {
		tokens = Tokens;
	}

	string getRule() {
		return rule;
	}

	void setRule(string Rule) {
		rule = Rule;
	}

	string getSuggestion() {
		return suggestion;
	}

	void setSuggestion(string Suggestion) {
		suggestion = Suggestion;
	}
};


#endif // !RULE