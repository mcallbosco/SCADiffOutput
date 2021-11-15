#ifndef ANTLR_SERVER_
#define ANTLR_SERVER_

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>

using namespace std;

class ANTLR_Server {
private:
	string cppFile;
	char* homeDir;
	string antlrWorkingDir = "/ANTLR";
	string commandBeg = "java org.antlr.v4.gui.TestRig CPP14 translationUnit -tree ";
	string commandEnd = " 2> ~/SCA/SCA/core/out/error.txt 1> ~/SCA/SCA/core/out/tree.txt ";

public:
	ANTLR_Server() {};
	ANTLR_Server(string& cppfilepath);
	bool serveCode(string& treeTxtFile, string& errorTxtFile);
	void printErrors();
};

#include "ANTLR_Server.cpp"

#endif // !ANTLR_Server_
