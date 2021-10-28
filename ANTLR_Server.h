#ifndef ANTLR_SERVER_
#define ANTLR_SERVER_

#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

using namespace std;

class ANTLR_Server {
private:
	string cppFile;
	string antlrWorkingDir = "C:\\Users\\Public\\ANTLR\\cpp_grammar";
	string commandBeg = "grun CPP14 translationUnit -tree ";
	string commandEnd = " > \"C:\\Users\\Public\\ANTLR\\antlr_output\\tree.txt\" 2> \"C:\\Users\\Public\\ANTLR\\antlr_output\\error.txt\"";

public:
	ANTLR_Server() {};
	ANTLR_Server(string& cppfilepath) : cppFile(cppfilepath) {};
	bool serveCode(string& treeTxtFile, string& errorTxtFile);
	void printErrors();
};

#include "ANTLR_Server.cpp"

#endif // !ANTLR_Server_
