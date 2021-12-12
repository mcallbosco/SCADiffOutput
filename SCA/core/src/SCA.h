#ifndef SCA_
#define SCA_
#include "SCAInterface.h"

#include "Node.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <fstream>
#include <vector>
#include "ANTLR_Server.h"
#include "Component.h"
#include "AST_Parser.h"
#include "TemplateTable.h"
#include "Template_Matcher.h"
#include "createHTMLFile.h"
#include "Tree.h"
#include "TemplateTable.h"
#include "Template_Matcher.h"
#include "While_Loop.h"
#include "ForLoop.h"
#include "Switch.h"
#include "If.h"

using namespace std;

class SCA : public SCAInterface {

private:
	string cppFilePath;
	string templateTableFile;
	string htmlDir;
	Tree* ast;
	vector<Node*> iterationNodes;
	vector<Node*> selectionNodes;
	vector<While_Loop*> whileLoopComponents;
	vector<ForLoop*> forLoopComponents;
	vector<If*> ifComponents;
	vector<Switch*> switchComponents;

public:
	SCA();
	SCA(const string& pathToCppFile);
	SCA(const string& pathToCppFile, const string& pathToTemplateTable);
	SCA(const string& pathToCppFile, const string& pathToTemplateTable, const string& pathToHtmlDir);
	bool existsFile(string filePath) const;
	bool loadTemplateTable(string templateTableFile);
	string loadSourceCode(string filePath);
	bool serveCodeToANTLR(string& treeTxtFilePath, string& errorTxtFilePath);
	Node* readANTLROutputTree(string& treeTxtFilePath);
	bool readANTLROutputErrors(string& errorTxtFilePath);
	string matchTemplateWithTree();
	string createHTMLFile(string& matchedSuggestions);
};

#include "SCA.cpp"

#endif // !SCA_
