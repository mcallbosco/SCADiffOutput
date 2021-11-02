#include "ANTLR_Server.h"

bool ANTLR_Server::serveCode(string& treeTxtFile, string& errorTxtFile) {
	// build string to run ANTLR command
	string cmdString = commandBeg + cppFile + commandEnd;

	// set current working directory
	if (SetCurrentDirectoryA(antlrWorkingDir.c_str())) {
		//run command
		int errStatus = system(cmdString.c_str());
		if (errStatus == 0) {
			treeTxtFile = "C:\\Users\\Public\\ANTLR\\antlr_output\\tree.txt";
			errorTxtFile = "C:\\Users\\Public\\ANTLR\\antlr_output\\error.txt";
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
	ifstream errorFile("C:\\Users\\Public\\ANTLR\\antlr_output\\error.txt");
	cout << "Error(s): " << endl;

	if (errorFile.is_open()) {
		while (getline(errorFile, line)) {
			cout << line << endl;
		}
	}
	else { cout << "Could not open error file..." << endl; }
	errorFile.close();
}
