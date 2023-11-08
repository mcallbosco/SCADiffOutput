#include <iostream>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include "SCA.h"

using namespace std;

string matchedSuggestions;
string htmlFilePath;
string treeTxtFilePath;
string errorTxtFilePath;
string templateTableFile = "/SCA/SCA/core/dat/rules.txt";
string sourceFileDir = "/SCA/SCA/user/source-code";
string htmlFileDir = "/SCA/SCA/user/output";
char *homeDir;

void explore(char *dir_name, string inputPath, string outputPath);

int main(int argc, char *argv[]) {
	// get user home directory
	struct passwd *pw = getpwuid(getuid());
	homeDir = pw->pw_dir;

	// update absolute paths for templateTableFile and sourceFileDir
	sourceFileDir = string(homeDir) + sourceFileDir;
	string sourceFile;
	string outputPath;
	if (argc >= 3){
		sourceFile = argv[1];
		outputPath = argv[2];
	}
	else{
		sourceFile = "";
		outputPath = "";
	}

	templateTableFile = string(homeDir) + templateTableFile;
	htmlFileDir = string(homeDir) + htmlFileDir;
	
	explore((char*)sourceFileDir.c_str(), sourceFile, outputPath);
	
	
	// open /output in file explorer when SCA is complete
	system("xdg-open ~/SCA/SCA/user/output");

	return 0;
}

void explore(char *dir_name, string inputFile = "", string outputPath = "") {

	DIR *dir;
	struct dirent *entry;
	struct stat info;
	bool singleFile = false;
	bool multiFile = false;

	// open directory
	if (inputFile != "") {
		singleFile = true;
		dir = NULL;
	}
	else {
		dir = opendir(dir_name);
		multiFile = true;
	}
	
	if (!dir && inputFile == "") {
		cout << "Unable to open directory => " << dir_name << endl;
		cout << inputFile << endl;
		return;
	}
	else {
		while(singleFile == true || (multiFile && (entry = readdir(dir)) != NULL)) {
			string cppFilePath = "";
			if (singleFile == true){
				cppFilePath = inputFile;
				cout << inputFile << endl;
			}
			else {
				cppFilePath = sourceFileDir + "/" + string(entry->d_name);
			}
			
			if (cppFilePath.substr(cppFilePath.length() - 3, 3) == "cpp") {
				SCA* sca = new SCA(cppFilePath, templateTableFile, htmlFileDir);
				Node* rt;

				sca->existsFile(cppFilePath);
				//cout << "File exists\n";
				
				sca->loadTemplateTable(templateTableFile);
				//cout << "Loaded template table\n";
				
				cppFilePath = sca->loadSourceCode(cppFilePath);
				//cout << "Loaded Source Code\n";
				cout << cppFilePath << endl;

				if (cppFilePath == "failed") {
					goto SkipFile;
				}
				sca->serveCodeToANTLR(treeTxtFilePath, errorTxtFilePath);
				//cout << "Served code to ANTLR\n";

				// get root
				rt = sca->readANTLROutputTree(treeTxtFilePath);
				//cout << "Read tree text file into memory\n";

				sca->readANTLROutputErrors(errorTxtFilePath);
				//cout << "Read output error file\n";

				matchedSuggestions = sca->matchTemplateWithTree();
				//cout << "Matched suggestions with tree\n";

				htmlFilePath = sca->createHTMLFile(matchedSuggestions, outputPath);
				//cout << "Created html file\n";
				cout << "Successfully analyzed file: " << cppFilePath << endl << endl;
			}
			else {
				cout << "File: " + cppFilePath + " not recognized.\n";
				cout << "Make sure file is named with proper extenstion (.cpp)\n\n";
			}
			if (false) {
				SkipFile:
				cout << "Loading source code failed... skipping file: " << cppFilePath << endl << endl;
			}
			singleFile = false;
		}
	}
	closedir(dir);
}