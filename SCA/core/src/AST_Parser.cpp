#include "AST_Parser.h"
#include <map>
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
	getline(cppFile, line);
	lineNumber++;
	_traverseInOrder(rawAST->getRoot());

}



void AST_Parser::_searchLineForToken(Node* rt) {
    // Get the token data from the leaf node
    string token = rt->getData();

    // Reset the line number counter
    lineNumber = 0;

    // Rewind the file position to the beginning
    cppFile.clear();
    cppFile.seekg(0, ios::beg);

    // Variable to store the current line
    string currLine;

	//Variable to store the times the current token has been found
	int tokenCount = 0;
	tokenCount = readTokensDict[token];

    // Iterate over the lines of the file
	int counter = 0;
    while (getline(cppFile, currLine)) {
		if (counter < tokenCount) {
			counter++;
			continue;
		}
        lineNumber++;
        // Search for the token in the current line
        size_t pos = currLine.find(token);
        if (pos != string::npos) {
            // Token found, set the line number in the leaf node
            rt->setLineNum(lineNumber);
			readTokensDict[token] = tokenCount + 1;
            break;
        }
    }
}

// traverse tree in order and search each leaf for its associated line number
void AST_Parser::_traverseInOrder(Node* rt) {
	Node* nodeIter = rt;
	if (nodeIter->getChildCount() == 0) {
		_searchLineForToken(nodeIter);
		return;
	}

	int totalChildren = nodeIter->getChildCount();

	for (int i = 0; i < totalChildren; i++) {
		_traverseInOrder(nodeIter->getChild(i));
	}
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
