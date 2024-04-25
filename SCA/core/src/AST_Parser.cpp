#include "AST_Parser.h"
using namespace std;


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
	// get token data
	string token = rt->getData();

	// check for comments and token in current line
	int commentFound = line.find("//");
	int found = line.find(token);

	if (found == -1){
		std::cout << "Token: " << token << "not found in " << line << std::endl;
	}

	// if found, add line number info to node
	if (found != -1 && commentFound == -1) {
		if (found < line.size()) {
			// remove token that was found from string line
			line = line.substr(found + token.size(), line.size() - 1);
		}

		rt->setLineNum(0);
		return;
	}
	// if not found go get next line and search again
	// HOW TO HANDLE ERRORS??? <missing ' '> && OTHER LEAF NODES THAT AREN'T IN SOURCE CODE???
	getline(cppFile, line);
	lineNumber++;
	_searchLineForToken(rt);
}

void AST_Parser::_handleMatchError(Node* rt, const string& token) {
	int isHashTag = line.find("#");
	int isSingleLineComment = line.find("//");
	int isMultiLineComment = line.find("/*");
	
	// if there's a <missing ' '> token or <EOF> 'end of file' token
	if ((token.size() >= 5 && token.substr(0, 5) == "<miss") || token == "<EOF>" || token == "<EOF>\n") {
		rt->setLineNum(1);
	}
	// else if the line contains a preprocessor directive
	else if (isHashTag != -1) {
		// checks if '#' is in a single line comment
		if (isSingleLineComment != -1 && (isSingleLineComment < isHashTag)) {
			goto handle_single_line_comment;
		}
		else if (isMultiLineComment != -1 && (isMultiLineComment < isHashTag)) {
			goto handle_multi_line_comment;
		}
		getline(cppFile, line);
		lineNumber++;
		_searchLineForToken(rt);
	}
	// else if the line is a comment
	else if (isSingleLineComment != -1) {
		handle_single_line_comment:
		// check to make sure the comment is not preceded by code (ex: int i = 2; //some comment)
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '\t' || line[i] == space) {}
			else if (line.size() - 1 >= i + 1 && line[i] == '/' && line[i + 1] == '/') {
				getline(cppFile, line);
				lineNumber++;
				_searchLineForToken(rt);
				break;
			}
			// comment is not first token in line, handle other tokens before comment
			else goto jumpToOtherMismatch;
		}
	}
	else if (isMultiLineComment != -1) {
		handle_multi_line_comment:
		// make sure comment is not preceded by code (ex: int i = 2; /* comment */
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == '\t' || line[i] == space) {}
			else if (line.size() - 1 >= i + 1 && line[i] == '/' && line[i + 1] == '*') {
				while (true) {
					if (line.find("*/") != string::npos) {
						int foundPos = line.find("*/");
						line = line.substr(foundPos + 2, line.size() - 1);
						_searchLineForToken(rt);
						break;
					}
					else {
						getline(cppFile, line);
						lineNumber++;
					}
				}
			}
			// comment is not first token in line, handle other tokens before comment
			else goto jumpToOtherMismatch;
		}
	}
	// else there is some other mismatch
	else {
	jumpToOtherMismatch:
		// obtain current file position before getting new line
		int filePosition = cppFile.tellg();
		// store current line in temp in case it needs to be revisited
		string temp = line;
		if (getline(cppFile, line)) {
			// if line is blank increase lineNumber and search again
			if (line.empty()) {
				lineNumber++;
				_searchLineForToken(rt);

			}
			// else line is not blank, but may be filled with '\t' or '\n' or spaces
			else {
				// check to make sure line is not blank
				bool isLineBlank = true;
				for (int i = 0; i < line.size(); i++) {
					if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {}
					else {
						isLineBlank = false;
						break;
					}
				}
				if (isLineBlank) {
					lineNumber++;
					_searchLineForToken(rt);
					goto end_of_handle_error;
				}
				else {
					int found = line.find(token);
					int commentFound = line.find("//");
					int multiLineCommentFound = line.find("/*");
					if (multiLineCommentFound != -1) {
						lineNumber++;
						goto handle_multi_line_comment;
					}
					else if (found == -1 && commentFound == -1) {
						rt->setLineNum(2);
						cppFile.seekg(filePosition);
						line = temp;
					}
					else if (found == -1 && commentFound != -1) {
						lineNumber++;
						_searchLineForToken(rt);
						goto end_of_handle_error;

					}
					else if (found != -1 && commentFound == -1) {
						lineNumber++;
						rt->setLineNum(3);
						line = line.substr(found + token.size(), line.size() - 1);
					}
					else {
						if (found < commentFound) {
							lineNumber++;
							rt->setLineNum(4);
							line = line.substr(found + token.size(), line.size() - 1);
						}
						else {
							lineNumber++;
							_searchLineForToken(rt);
							goto end_of_handle_error;
						}
					}
				}
			}
		}
		else {
			rt->setLineNum(5);
		}

	}
end_of_handle_error: ;	
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
