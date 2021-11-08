#ifndef CREATE_HTML_FILE
#define CREATE_HTML_FILE

#include <iostream>
#include <fstream>
#include <string>
#include "SCA.h"

class createHTML {
private:
	string htmlLocation;
	string htmlName = "results.html";
	string cppFileLocation;
	string Suggestions;
public:
	//constructor
	createHTML();

	createHTML(string HTMLlocation, string cppLocation, string suggestions);

	//gets cpp file and assisgns to string cppFile.
	void setCPPfile(string CPPfile);

	//sets Suggestions to suggestions gotten from matching function.
	void setSuggestions(string suggestions);

	//sets where the html file will be saved.
	void setHTMLlocation(string HTMLlocation);

	//creates html file from cpp file and suggestions.
	void makeHTMLfile();

	//creates css file for html in same directory html file was made in.
	void makeCSSfile();
};

createHTML::createHTML() {
}

createHTML::createHTML(string HTMLlocation, string cppLocation, string suggestions) {
	htmlLocation = HTMLlocation;
	cppFileLocation = cppLocation;
	Suggestions = suggestions;
}

void createHTML::setSuggestions(string suggestions) {
	Suggestions = suggestions;
}

void createHTML::setCPPfile(string CPPfile) {
	cppFileLocation = CPPfile;
}

void createHTML::setHTMLlocation(string HTMLlocation) {
	htmlLocation = HTMLlocation;
}

void createHTML::makeHTMLfile()
{
	//create html file
	ofstream htmlFile(htmlLocation + "\\" + htmlName);

	//start of file
	htmlFile << "<!DOCTYPE html>\n" << "<html>\n" << "\t<head>\n";

	//css link
	htmlFile << "\t\t<link rel=\"stylesheet\" href=\"htmlStyle.css\"\n>";

	htmlFile << "\t</head>\n" << "\t<body>\n";

	//format columns
	htmlFile << "\t\t<div id=container-for-columns class=\"columnContainer\"\n>";

	ifstream cppFile(cppFileLocation);
	string CPPstring;
	string tempString;
	int lineNum = 1;

	while (getline(cppFile, tempString))
	{
		CPPstring += lineNum + "   " + tempString + "\n";
		lineNum++;
	}

	//load soucre code in left column
	htmlFile << "\t\t\t<p>" << CPPstring << "<p>\n";
	//load suggestions in right column
	htmlFile << "\t\t\t<p>" << Suggestions << "<p>\n";
}

void createHTML::makeCSSfile() 
{
	ofstream  cssFile(htmlLocation + "//htmlStyle.css");

	cssFile << ".columnContainer{\n" << "\tdisplay: flex;\n" << "}\n";
	cssFile << "p{\n" << "\tflex: 1;\n" << "}\n";
}

#endif // !CREATE_HTML_FILE