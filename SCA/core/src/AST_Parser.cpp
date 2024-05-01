#include "AST_Parser.h"
#include <map>
#include <list>

using namespace std;



std::map<std::string, std::int16_t> readTokensDict = {};

bool AST_Parser::testFileExists(const string& file) {
	ifstream test(file);
	if (test) {
		return true;
	}
	else {
		return false;
	}
}

bool AST_Parser::parseTree() {
	astTxt.open(treeTxtFilePath);
	bool isReady = false;
	if (astTxt.is_open()) {
		// adjust seek position to start of tree in txt file
		if (astTxt.peek() != leftPar) {
			while (!isReady && getline(astTxt, word, newLine)) {
				if (astTxt.peek() == leftPar) { isReady = true; }
			}
		}
		else {
			isReady = true;
		}

		if (isReady) {
			while (getline(astTxt, word, space)) {
				currFilePos = astTxt.tellg();
				switch (word[0]) {
					// word = "(xxxxxx" 
				case leftPar:
					_parseNewParent();
					break;
					// word = "<xxxxx"
				case leftAngleBracket:
					_parseError();
					break;
					// word = ("xxxxx")
				case doubleQuote:
					_parseString();
					break;
					// word = "xxxxx"
				default:
					_parseLeaf();
					break;
				}
			}
		}
		else {
			// if unsuccessful close file and return false
			astTxt.close();
			cout << "Check txt file... unfamiliar format detected." << endl;
			return false;
		}
		// if successful close file and return true
		astTxt.close();
		return true;
	}
	else { 
		cout << "Couldn't open file... check path.";
		return false; 
	}
}

void AST_Parser::_parseNewParent() {

	if (word.size() == 1) { //single left parentheses--> add leaf
		rawAST->addLeaf(word);
	}
	// word = "(xxxxxx"
	else {
		rawAST->addParent(word.substr(1, word.size() - 1));
	}
}

void AST_Parser::_parseString() {
	string bytes = "";
	int numOfPops = 0;
	char byte = 0;

	// seek back to beginning of string and parse through it
	astTxt.seekg(currFilePos - word.size() - 1);
	// get first byte, place in string
	astTxt.get(byte);
	bytes += byte;

	// loop through remainder of string watching for string end
	while (astTxt.get(byte)) {
		if (byte == doubleQuote) {
			if (astTxt.peek() == rightPar) {
				if (bytes[bytes.size() - 1] == escapeSeq && bytes[bytes.size() - 2] != escapeSeq) {
					goto continueStringParse;
				}
				else if (bytes[bytes.size() - 1] == escapeSeq && bytes[bytes.size() - 2] == escapeSeq) {
					bytes += byte;
					break;
				}
				else {
					bytes += byte;
					break;
				}
				
			}
		}
		continueStringParse:
		bytes += byte;
	}

	// Add leaf and determine # of pops
	rawAST->addLeaf(bytes);
	// determine # of pops
	while (astTxt.get(byte)) {
		if (astTxt.peek() == space) {
			astTxt.get(byte); // eat the space so file is in correct position for next iteration
			numOfPops++; // increase numOfPops to account for last ")"
			break;
		}
		else if (byte == rightPar) {
			numOfPops++;
		}
	}

	// popBack to appropriate parent
	for (int i = 0; i < numOfPops; i++) {
		rawAST->popBackToParent();
	}
}

void AST_Parser::_parseError() {
	// no error just '<' character
	if (word.size() == 1) {
		rawAST->addLeaf(word);
	}
	// no error (missing token) => hand control to _parseLeaf()
	else if (word != "<missing") {
		_parseLeaf();
	}
	else {
		string bytes = " ";
		int numOfPops = 0;
		char byte = 0;
		bool isEndOfError = false;
		// parse error character by character
		while (!isEndOfError && astTxt.get(byte)) {
			// if "> "
			if (byte == rightAngleBracket && astTxt.peek() == space) {
				bytes += byte;
				isEndOfError = true;
				// fetch the space so file position is correct for next iteration
				astTxt.get(byte);
			}
			// else if ">)" or ">))"
			else if (byte == rightAngleBracket && astTxt.peek() == rightPar) {
				while (astTxt.get(byte) && !isEndOfError) {
					if (astTxt.peek() == space) {
						isEndOfError = true;
					}
					numOfPops++;
				}
			}
			// ...still parsing error message
			else {
				bytes += byte;
			}
		}
		// append remainder of error message to word
		word += bytes;
		// add error message to leaf node
		rawAST->addLeaf(word);
		// pop traveler back to correct parent node
		for (int i = 0; i < numOfPops; i++) {
			rawAST->popBackToParent();
		}
	}
}

void AST_Parser::_parseLeaf() {
	// if last character in word is a ")" and first char is NOT ")"
	if (word[word.size() - 1] == rightPar && word[0] != rightPar) { 
		int numOfPops = 0;
		for (int i = word.size() - 1; i >= 0; i--) {
			if (word[i] == rightPar) {
				numOfPops++;
			}
			else {
				// right Parentheses are complete, exit loop
				break;
			}
		}
		rawAST->addLeaf(word.substr(0, word.size() - numOfPops)); //add string that appears before parentheses
		for (int i = 0; i < numOfPops; i++) { //pop traveler back to parent
			rawAST->popBackToParent();
		}
	}
	// word = ")" or ")...)"
	else if (word[word.size() - 1] == rightPar && word[0] == rightPar) { //only executes if all characters are "))))))"
		// word = ")...)"
		if (word.size() > 1) {
			rawAST->addLeaf(")");
			for (size_t i = 0; i < word.size() - 1; i++) {
				rawAST->popBackToParent();
			}
		}
		// word = ")"
		else {
			rawAST->addLeaf(")");
		}
	}
	// word is the <EOF> delimiter
	else if (word.substr(0, 5) == endOfFile) {
		rawAST->addLeaf(word.substr(0, 5));
	}
	// word contains newline "\n" at the end of its string
	else if (word.size() >= 2 && word.substr(word.size() - 1, word.size()) == "\n") {
		rawAST->addLeaf(word.substr(0, word.size() - 2));
	}
	// word = "xxxx" => no leading left parentheses or trailing right parentheses
	else {
		rawAST->addLeaf(word);
	}
}

void AST_Parser::condenseTree(Node* rt) {
	Node* iterNode = rt;
	Node* parent = rt->getParent();
	Node* child = nullptr;

	// only get child ptr if one exists
	if (rt->getChildCount() > 0) {
		child = rt->getChild(0);
	}

	// base case: recurse up the tree when leaf is reached
	if (rt->getChildCount() == 0) {
		return;
	}
	// check if node is candidate for removal, if so -> remove node
	else if (parent != nullptr) {
		if (parent->getChildCount() == 1 && child->getChildCount() >= 1 && iterNode->getChildCount() == 1)
			iterNode = rawAST->removeParent(rt);
	}


	int totalChildren = iterNode->getChildCount();
	// continue recursion down the tree
	for (int i = 0; i < totalChildren; i++) {
		condenseTree(iterNode->getChild(i));
	}
}


void AST_Parser::getNodeLineNums() {
	cppFile.open(cppFilePath);
	cppFile.clear();
    cppFile.seekg(0, ios::beg);
	vector<string> cppFileContent;
	while (getline(cppFile, line)) {
		cppFileContent.push_back(line);
	}

	//scan through to look for comments and remove them
	//iterate through the list of strings
	int length = cppFileContent.size();
	bool isMultiLineComment = false;
	for (int i = 0; i < length; i++) {
		//check if the string has a comment
		size_t commentPos = cppFileContent[i].find("//");
		if (commentPos != string::npos) {
			cppFileContent[i] = cppFileContent[i].substr(0, commentPos);
		}
		//check if the string has a #
		size_t hashPos = cppFileContent[i].find("#");
		if (hashPos != string::npos) {
			cppFileContent[i] = cppFileContent[i].substr(0, hashPos);
		}
		//check if the string has a /*
		size_t multiLineCommentStart = cppFileContent[i].find("/*");
		if (multiLineCommentStart != string::npos) {
			size_t multiLineCommentEnd = cppFileContent[i].find("*/");
			if (multiLineCommentEnd != string::npos) {
				cppFileContent[i] = cppFileContent[i].substr(0, multiLineCommentStart) + cppFileContent[i].substr(multiLineCommentEnd + 2);
			}
		}
	}
	cppFile.open(cppFilePath);
	getline(cppFile, line);
	lineNumber++;
	_traverseInOrder(rawAST->getRoot(), cppFileContent);

}



vector<string> AST_Parser::_searchLineForToken(Node* rt, vector<string> cppFileList) {
    // Get the token data from the leaf node
    string token = rt->getData();
    lineNumber = 0;
    // Variable to store the current line
    string currLine;

    // Iterate over the lines of the file
	int counter = 0;
	int lengthOfCppFile = cppFileList.size();

	for (int i = 0; i < lengthOfCppFile; i++) {
		std::cout << "Counter: " << counter << std::endl;
		currLine = cppFileList[i];
		lineNumber++;
		// Check if the line contains the token
		size_t found = currLine.find(token);
		if (found != string::npos) {
			// Remove the token from the line
			cppFileList[i].erase(found, token.length());
			// Insert the line number into the leaf node
			rt->setLineNum(lineNumber);
			// Increment the token count
			return cppFileList;
		}
	}


	return cppFileList;
}

// traverse tree in order and search each leaf for its associated line number
vector<string> AST_Parser::_traverseInOrder(Node* rt, vector<string> cppFileContent) {
	
	Node* nodeIter = rt;
	if (nodeIter->getChildCount() == 0) {
		cppFileContent = _searchLineForToken(nodeIter, cppFileContent);
		return cppFileContent;
	}
	int totalChildren = nodeIter->getChildCount();
	for (int i = 0; i < totalChildren; i++) {
		cppFileContent = _traverseInOrder(nodeIter->getChild(i), cppFileContent);
	}
	return cppFileContent;
}

void AST_Parser::inOrderPrintAllLeafData(Node* rt) {
	Node* nodeIter = rt;
	if (nodeIter->getChildCount() == 0) {
		_printInOrder(rt);
		return;
	}

	int totalChildren = nodeIter->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		inOrderPrintAllLeafData(nodeIter->getChild(i));
	}
}

void AST_Parser::_printInOrder(Node* currNode) {
	if (lineNums.empty()) {
		lineNums.push_back(currNode->getLineNum());
		cout << currNode->getData() << "(" << currNode->getLineNum() << ") ";
	}
	else {
		lineNums.push_back(currNode->getLineNum());
		if (lineNums[lineNums.size() - 1] != lineNums[lineNums.size() - 2]) {
			cout << endl;
		}
		cout << currNode->getData() << "(" << currNode->getLineNum() << ") ";
	}

}

Node* AST_Parser::getTreeRoot() {
	return rawAST->getRoot();
}

void AST_Parser::printTreeGraphically(Node* rt, vector<bool> flag, int depth = 0, bool isLast = false) {
	if (rt->getChildCount() == 0) {
		return;
	}

	for (int i = 1; i < depth; i++) {
		if (flag[i] == true) {
			cout << "| " << " " << " " << " ";
		}
		else {
			cout << " " << " " << " " << " ";
		}
	}

	if (depth == 0) {
		cout << rt->getData() << endl;
	}
	else if (isLast) {
		cout << "+--- " << rt->getData() << endl;
		flag[depth] = false;
	}
	else {
		cout << "+--- " << rt->getData() << endl;
	}

	int it = 0;
	for (auto i = 0; i < rt->getChildCount(); i++, it++) {
		printTreeGraphically(rt->getChild(i), flag, depth + 1, it == (rt->getChildCount() - 1));
		flag[depth] = true;
	}
}

Tree* AST_Parser::getTree() {
	return rawAST;
}
