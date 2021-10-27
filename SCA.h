#ifndef SCA_
#define SCA_
#include "SCAInterface.h"
#include "ANTLR_Server.h"
#include "AST_Parser.h"
#include "Tree.h"
#include <iostream>

using namespace std;

class SCA : public SCAInterface {
private:
	string cppFilePath;
	string templateTableFile;
	Tree* ast;

public:
	SCA();
	SCA(const string& pathToCppFile);
	SCA(const string& pathToCppFile, const string& pathToTemplateTable);
	bool existsFile(string filePath) const;
	bool loadTemplateTable(string templateTableFile);
	string loadSourceCode(string filePath);
	bool serveCodeToANTLR(string& treeTxtFilePath, string& errorTxtFilePath);
	Node* readANTLROutputTree(string& treeTxtFilePath);
	bool readANTLROutputErrors(string& errorTxtFilePath);
	string matchTemplateWithTree() const;
	string createHTMLFile();
};

#include "SCA.cpp"

#endif // !SCA_

