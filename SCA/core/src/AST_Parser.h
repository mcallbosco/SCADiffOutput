#ifndef AST_PARSER_
#define AST_PARSER_
#include <iostream>
#include <fstream>
#include <string>
#include "Tree.h"

using namespace std;

//Constants
const char space = ' ';
const char leftPar = '(';
const char rightPar = ')';
const char leftCurlyBrace = '{';
const char leftAngleBracket = '<';
const char rightAngleBracket = '>';
const char doubleQuote = 34;
const char escapeSeq = '\\';
const char newLine = '\n';
const string endOfFile = "<EOF>";


class AST_Parser {
private:
	string treeTxtFilePath;
	string cppFilePath;
	ifstream astTxt;
	ifstream cppFile;
	string line;
	int lineNumber;
	streamoff currFilePos;
	string word;
	Tree* rawAST = new Tree();

	// for printing in Order
	vector<int> lineNums;

	//private member function, inserts nodes
	void _parseNewParent();
	void _parseString();
	void _parseError();
	void _parseLeaf();

public:
	// Constructors
	AST_Parser() : treeTxtFilePath(""), cppFilePath(""), lineNumber(0), currFilePos(0) {}
	AST_Parser(const string& fname) : treeTxtFilePath(fname), cppFilePath(""), lineNumber(0), currFilePos(0) {}
	AST_Parser(const string& fname, const string& cName) : treeTxtFilePath(fname), cppFilePath(cName), lineNumber(0), currFilePos(0) {}
	
	// Parse text file / test if file exists
	bool testFileExists(const string& file);
	bool parseTree();

	// Condense tree and helper function
	void condenseTree(Node* rt);

	// get line numbers from cppFile & helper functions
	void getNodeLineNums();
	vector<string> _searchLineForToken(Node* rt, vector<string> cppFileList);
	void _traverseInOrder(Node* rt);
	void _handleMatchError(Node* rt, const string& token);

	// print in order w/ line numbers
	void inOrderPrintAllLeafData(Node* rt);
	void _printInOrder(Node* currNode);

	// Get root node of tree
	Node* getTreeRoot();

	// Get pointer to tree
	Tree* getTree();

	//Print graphically
	void printTreeGraphically(Node* rt, vector<bool> flag, int depth, bool isLast);

};

#include "AST_Parser.cpp"

#endif // !AST_PARSER_

