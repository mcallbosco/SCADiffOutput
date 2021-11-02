#include <iostream>
#include <string>
#include "SCA.h"

using namespace std;

string matchedSuggestions;
string htmlFilePath;
string treeTxtFilePath;
string errorTxtFilePath;
string templateTableFile = "\"E:\College\Fall_2021\CSIT-499\matchTreeWithTemplate (Tyler)\\rules.txt\"";
string cppFilePath = "E:\\College\\Fall_2021\\CSIT-499\\ANTLR\\cppFiles\\errors.cpp";

int main() {


	SCA* sca = new SCA(cppFilePath, templateTableFile);
	Node* rt;

	sca->existsFile(cppFilePath);
	
	sca->loadTemplateTable(templateTableFile);
	
	cppFilePath = sca->loadSourceCode(cppFilePath);

	sca->serveCodeToANTLR(treeTxtFilePath, errorTxtFilePath);

	// get root
	rt = sca->readANTLROutputTree(treeTxtFilePath);

	sca->readANTLROutputErrors(errorTxtFilePath);

	matchedSuggestions = sca->matchTemplateWithTree();

	htmlFilePath = sca->createHTMLFile();


	

	return 0;
}