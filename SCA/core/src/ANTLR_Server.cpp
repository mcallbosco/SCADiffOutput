#include "ANTLR_Server.h"

ANTLR_Server::ANTLR_Server(string& cppfilepath) {
	// get user home directory
	struct passwd *pw = getpwuid(getuid());
	homeDir = pw->pw_dir;
	antlrWorkingDir = string(homeDir) + antlrWorkingDir;
	cppFile = cppfilepath;
}

bool ANTLR_Server::serveCode(string& treeTxtFile, string& errorTxtFile) {
	// build string to run ANTLR command
	string cmdString = commandBeg + cppFile + commandEnd;

	// set current working directory
	if (chdir(antlrWorkingDir.c_str()) == 0) {
		//run command
		int errStatus = system(cmdString.c_str());
		if (errStatus == 0) {
			treeTxtFile = string(homeDir) + "/SCA/core/out/tree.txt";
			errorTxtFile = string(homeDir) + "/SCA/core/out/error.txt";
		}
		else {
			printErrors();
			return false;
		}

		return true;
	}
	else {
		cout << "Could not locate directory: " << antlrWorkingDir << endl;
		return false;
	}

}

void ANTLR_Server::printErrors() {
	string line;
	string errorFilePath = string(homeDir) + "/SCA/core/out/error.txt";
	ifstream errorFile(errorFilePath);
	cout << "Error(s): " << endl;

	if (errorFile.is_open()) {
		while (getline(errorFile, line)) {
			cout << line << endl;
		}
	}
	else { cout << "Could not open error file..." << endl; }
	errorFile.close();
}
