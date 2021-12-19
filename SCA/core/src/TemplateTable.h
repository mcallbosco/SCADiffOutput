#ifndef TEMPLATE_TABLE
#define TEMPLATE_TABLE

#include "SCA.h"
#include "Rule.h"
#include "Component.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class TemplateTable {
private:
	//The template table
	unordered_map <int, Rule> template_table;

public:
	//constructor
	TemplateTable();

	//Loads template table into unordered_map from file
	bool loadTemplateTable(string templateTableFile);

	//returns the templateTable
	unordered_map <int, Rule> getTemplateTable();
};

TemplateTable::TemplateTable() {
}


bool TemplateTable::loadTemplateTable(string templateTableFile)
{
	//open template_table file
	ifstream template_file;
	template_file.open(templateTableFile);

	if (!template_file.is_open()) {
		cout << "Template table file was not found." << endl;
		return false;
	}

	//variables for loop
	string line;
	int i = 0;
	int Key;
	int Tokens;

	// skip the first line (first line contains the version number)
	getline(template_file, line);

	while (getline(template_file, line))
	{
		
		//seperating lines from file
		string key = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);
		string tokens = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);
		string rule = line.substr(0, line.find(","));
		line.erase(0, line.find(",") + 1);
		string suggestion = line.substr(0, line.find(","));

		//convert key and tokens to int
		Key = stoi(key);
		Tokens = stoi(tokens);

		Rule currRule;

		//set up Rule class
		currRule.setRuleNum(Key);
		currRule.setTokens(Tokens);
		currRule.setRule(rule);
		currRule.setSuggestion(suggestion);

		//insert line into unordered map
		template_table[i] = (Key, currRule);

		i++;
	}
	return true;
}

unordered_map <int, Rule> TemplateTable::getTemplateTable() {
	return template_table;
}

#endif // !TEMPLATE_TABLE