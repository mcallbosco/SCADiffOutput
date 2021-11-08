#include "SCA.h"

// Constructor
SCA::SCA(): ast(nullptr), templateTableFile(""), cppFilePath("") {};

// Constructor with parameter for cppFilePath
SCA::SCA(const string& pathToCppFile) {
	cppFilePath = pathToCppFile;
	ast = nullptr;
	templateTableFile = "";
}

SCA::SCA(const string& pathToCppFile, const string& pathToTemplateTable) {
	cppFilePath = pathToCppFile;
	ast = nullptr;
	templateTableFile = pathToTemplateTable;
}

bool SCA::existsFile(string filePath) const {
	ifstream testFile(filePath);
	if (testFile.is_open())
	{
		testFile.close();
		return true;
	}
	else
	{
		testFile.close();
		return false;
	}
}

// Undefined Function
bool SCA::loadTemplateTable(string templateTableFile) {
	TemplateTable* tempTableLoader = new TemplateTable();
	if (tempTableLoader->loadTemplateTable(templateTableFile))
		return true;
	else
		return false;
}


string SCA::loadSourceCode(string sourceCodeFileLocation)
{
	cout << "Loading source code..." << endl;

	if (!SCA::existsFile(sourceCodeFileLocation))
	{
		cout << "File not found or failed to open." << endl;
		return "failed";
	}

	ifstream sourceCodeFile;
	sourceCodeFile.open(sourceCodeFileLocation);

	cout << "File found." << endl;

	if (SCA::existsFile("Source.cpp"))									//Try to open "Source.cpp," which will be a file to append the function(s) to
	{
		cout << "Source.cpp found. Attaching functions..." << endl;
		ifstream problemFile;
		problemFile.open("Source.cpp");								

		vector<string> functions;
		vector<string> functionHeaders;
		int numOfFunctions = 0;
		string line;
		while (!sourceCodeFile.eof())	//Loop to end of file
		{
			getline(sourceCodeFile, line, ' ');	//Read up to first space, should catch the return type of a function header
			if (line == "int" || line == "void" || line == "bool" || line == "float" || line == "double")	//find keyword return type, these should be all beginners use
			{
				functions.push_back(line);								//Push a new string in the functions vector containing the return type
				getline(sourceCodeFile, line, ')');					//Read up to the closing parentheses for the args
				functions[numOfFunctions] += ' ' + line + ')';			//Add the rest of the header to the string, not forgetting the dropped delimiters
				functionHeaders.push_back(functions[numOfFunctions]);	//Push that into the functionHeaders vector
				getline(sourceCodeFile, line, '{');
				functions[numOfFunctions] += line;
				functions[numOfFunctions] += '{';

				//Reading in the function character by character, keeping track of the brackets to know when to stop
				stack<char> brackets;
				char character;
				brackets.push('{');

				while (!brackets.empty() && !sourceCodeFile.eof())	//Loop until the stack is empty or we reach the end of the file, the latter to prevent an infinite while loop
				{
					sourceCodeFile.get(character);					//Get a character
					functions[numOfFunctions] += character;			//Add the character to the function vector
					if (character == '{')							//Check if it's a left bracket
						brackets.push(character);					//Push it if it is
					else if (character == '}')
						brackets.pop();								//Pop if it's a right bracket
				}
				sourceCodeFile.ignore(256, '\n');					//Moves cleanly to the next line
				line = sourceCodeFile.get();
				numOfFunctions++;									//Increase the counter of how many functions there are
			}
			else
			{
				sourceCodeFile.ignore(256, '\n');					//Moves cleanly to the next line
				line = sourceCodeFile.get();
			}
		}

		if (numOfFunctions > 0)
		{
			ofstream newFile;
			newFile.open("new" + sourceCodeFileLocation);		//New file to print to

			bool* hasAPrototype = new bool[numOfFunctions];		//Array of bools signifying if the function already has a prototype in Source.cpp
			for (int i = 0; i < numOfFunctions; i++)			//Initialize it
				hasAPrototype[i] = false;

			int lines = 0, mainPos = 0;							//Keep track of lines for the purpose of knowing where main is

			while (!problemFile.eof())							//Loop to figure out what functions (if any) have prototypes already
			{
				getline(problemFile, line);
				for (int i = 0; i < numOfFunctions; i++)
				{
					if (line.find(functionHeaders[i]) != string::npos)
						hasAPrototype[i] = true;
				}
				if (line.find("int main") != string::npos)		//Additionally, figure out where the main is by line
					mainPos = lines;
				lines++;
			}

			problemFile.clear();
			problemFile.seekg(0);
			for (int i = 0; i < mainPos - 1; i++)				//Read in Source.cpp up to the line before the main function
			{
				getline(problemFile, line);
				newFile << line << endl;
			}
			newFile << "\n//New Function Headers \n" << endl;
			for (int i = 0; i < numOfFunctions; i++)			//Write function headers just before main, if there's not one already
			{
				if (!hasAPrototype[i])
					newFile << functionHeaders[i] << ';' << endl << endl;
			}
			while (!problemFile.eof())							//Get to the end of the file, reading it in to newFile (should just write main)
			{
				getline(problemFile, line);
				newFile << line << endl;
			}
			newFile << "\n//Function Declarations \n";
			for (int i = 0; i < numOfFunctions; i++)			//Write the function declarations at the end of the file
			{
				newFile << functions[i] << endl;
			}

			return "new" + sourceCodeFileLocation;

		}
		else
		{
			cout << "No functions found in input file." << endl;
			return "failed";
		}
	}

	cout << "Checking for main() funciton..." << endl;

	//Scans code for "int main()" to see if it's already declared
	string line;
	while (!sourceCodeFile.eof())
	{
		getline(sourceCodeFile, line);
		if (line.find("int main(") != string::npos)
		{
			cout << "main() found." << endl;
			//check for command line parameters??
			sourceCodeFile.close();
			return "existing";									//Return code for the case that main exists
		}
	}

	cout << "main() not found, creating main() function to test..." << endl;

	//Finding function headers
	sourceCodeFile.clear();
	sourceCodeFile.seekg(0);		//Go to front of the file
	int numOfFunctions = 0;			//Counter for how many functions there are for the purpose of dynamically allocating a function class array
	//Note: Likely not necessary if we use a vector instead of a dynamic array, but I already wrote this and I'm lazy.

	while (!sourceCodeFile.eof())	//Loop to end of file
	{
		getline(sourceCodeFile, line, ' ');	//Read up to first space, should catch the return type of a function header
		if (line == "int" || line == "void" || line == "bool" || line == "float" || line == "double")	//find keyword return type, these should be all beginners use
		{
			numOfFunctions++;						//Increase the counter of how many functions there are

			//Skipping passed the function definition using the curly braces
			stack<char> brackets;
			char character;
			getline(sourceCodeFile, line, '{');			//Read up to left bracket
			brackets.push('{');

			while (!brackets.empty() && !sourceCodeFile.eof())	//Loop until the stack is empty or we reach the end of the file, the latter to prevent an infinite while loop
			{
				sourceCodeFile.get(character);					//Get a character
				if (character == '{')							//Check if it's a left bracket
					brackets.push(character);					//Push it if it is
				else if (character == '}')
					brackets.pop();								//Pop if it's a right bracket
			}
			sourceCodeFile.ignore(256, '\n');					//Moves cleanly to the next line
			line = sourceCodeFile.get();
		}
		else
		{
			sourceCodeFile.ignore(256, '\n');					//Moves cleanly to the next line
			line = sourceCodeFile.get();
		}

	}

	//Pick out function names, parameter types
	if (numOfFunctions > 0)
	{
		string* functionNames = new string[numOfFunctions];	//Dynamically allocated array of function names
		vector<vector<string>> parameters(numOfFunctions);	//Using vectors for a dynamic two-dimensional array

		sourceCodeFile.clear();
		sourceCodeFile.seekg(0);							//Back to the top of the file

		int funcs = 0;										//iterator
		while (!sourceCodeFile.eof())
		{
			getline(sourceCodeFile, line, ' ');				//Same idea as the first loop, but this time we're more ready
			if (line == "int" || line == "void" || line == "bool" || line == "float" || line == "double")	//find keyword return type
			{
				getline(sourceCodeFile, functionNames[funcs], '(');		//Reads the name of the function into the array
				char tempCharacter = 'a';
				sourceCodeFile.get(tempCharacter);
				if (tempCharacter != ')')								//Check if we're at the end of the params
				{
					getline(sourceCodeFile, line, ' ');
					line = tempCharacter + line;
					parameters[funcs].push_back(line);
					while (tempCharacter != ')')						//Until we reach the right parenthese
					{
						sourceCodeFile.get(tempCharacter);
						if (tempCharacter == ',')						//Iterate every time there's a comma denoting a new parameter
						{
							getline(sourceCodeFile, line, ' ');			//Reads the variable type of the paramater into line
							parameters[funcs].push_back(line);			//Reads line into the vector of parameters
						}
					}
				}
				stack<char> brackets;						//Stack to read brackets and get to the end of the function declaration
				char character;
				getline(sourceCodeFile, line, '{');			//Read up to left bracket
				brackets.push('{');

				while (!brackets.empty() && !sourceCodeFile.eof())	//Loop until the stack is empty or we reach the end of the file, the latter to prevent an infinite while loop
				{
					sourceCodeFile.get(character);					//Get a character
					if (character == '{')							//Check if it's a left bracket
						brackets.push(character);					//Push it if it is
					else if (character == '}')
						brackets.pop();								//Pop if it's a right bracket
				}
				sourceCodeFile.ignore(256, '\n');
				line = sourceCodeFile.get();
				funcs++;											//Iterate amount of functions processed
			}
			else
			{
				sourceCodeFile.ignore(256, '\n');					//Moves cleanly to the front of the next line
				line = sourceCodeFile.get();
			}

		}

		//Dynamically create function calls
		string* functionCalls = new string[numOfFunctions];			//Dynamically allocated array of function call strings
		for (int i = 0; i < numOfFunctions; i++)
		{
			functionCalls[i] = functionNames[i] + "(";
			for (int j = 0; j < parameters[i].size(); j++)			//Loops for as many parameters as were found, could be none
			{
				if (j > 0)											//For every new parameter after the first, have to have a comma and space
					functionCalls[i] = functionCalls[i] + ", ";
				if (parameters[i][j] == "int" || parameters[i][j] == "double" || parameters[i][j] == "float")
				{
					functionCalls[i] = functionCalls[i] + "7";		//Uses the number 7 for any numerical parameter, can be randomized but I'm lazy
				}
				else if (parameters[i][j] == "string")				//Uses "sampleString" for any string parameter, perhaps can have a list of random strings
				{
					functionCalls[i] = functionCalls[i] + "\"sampleString\"";
				}
				else if (parameters[i][j] == "bool")
				{
					functionCalls[i] = functionCalls[i] + "false";	//Defaults to false for any boolean parameter
				}
			}
			functionCalls[i] = functionCalls[i] + ");";				//Close it off nice
			//Should be left with "functionName(param1, param2, ... paramN);"
		}

		//Make a new file containing the original functions and a new main function that calls all of them
		ofstream newCodeFile("new" + sourceCodeFileLocation);
		sourceCodeFile.clear();
		sourceCodeFile.seekg(0);				//Back to the top once more
		while (!sourceCodeFile.eof())
		{
			getline(sourceCodeFile, line);
			newCodeFile << line << endl;											//Copy the entire old file for the function declarations
		}
		sourceCodeFile.close();					//we're finished with you!

		newCodeFile << endl << endl;												//Add two empty lines
		newCodeFile << "int main()" << endl << "{" << endl << '\t';					//Define main
		for (int i = 0; i < numOfFunctions; i++)									//Write each function call
			newCodeFile << functionCalls[i] << endl << '\t';
		newCodeFile << endl << '\t' << "system(\"pause\");" << endl;				//System pause, this is optional
		newCodeFile << '\t' << "return 0;" << endl;									//Exit the program
		newCodeFile << "}";

		newCodeFile.close();

		return ("new" + sourceCodeFileLocation);

	}
	else													//Case that no functions are found, potentially an empty file
	{
		cout << "No functions found." << endl;
		sourceCodeFile.close();
		return "failed";
	}
}


bool SCA::serveCodeToANTLR(string& treeTxtFilePath, string& errorTxtFilePath) {
	ANTLR_Server* toAntlr = new ANTLR_Server(cppFilePath);

	if (toAntlr->serveCode(treeTxtFilePath, errorTxtFilePath))
		return true;
	else
		return false;
}


Node* SCA::readANTLROutputTree(string& treeTxtFilePath) {
	AST_Parser* ast_parser = new AST_Parser(treeTxtFilePath, cppFilePath);
	Node* rootPtr = nullptr;
	if (ast_parser->testFileExists(treeTxtFilePath)) {
		ast_parser->parseTree();
		rootPtr = ast_parser->getTreeRoot();
		ast_parser->condenseTree(rootPtr);
		ast_parser->getNodeLineNums();

		ast = ast_parser->getTree();
	}
	return rootPtr;
}

// Undefined Function
bool SCA::readANTLROutputErrors(string& errorTxtFilePath) {
	return true;
}

// Undefined Function
string SCA::matchTemplateWithTree() const {
	Template_Matcher templateMatcher;

	TemplateTable* tempTableLoader = new TemplateTable();
	tempTableLoader->loadTemplateTable(templateTableFile);

	templateMatcher.setTemplateTable(tempTableLoader->getTemplateTable());
	templateMatcher.setRulesArraySize();
	templateMatcher.checkTreeForErrors(ast->getRoot());

	return templateMatcher.outputSuggestions();
}

// Undefined Function
string SCA::createHTMLFile() {
	return "";
}