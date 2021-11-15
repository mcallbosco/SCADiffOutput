#ifndef CREATE_HTML_FILE
#define CREATE_HTML_FILE

#include <iostream>
#include <fstream>
#include <sstream>
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

	//left column
	htmlFile << "\t\t<div id=column1 class=\"columnContainer\"\n>";

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
	htmlFile << "\t\t\t<p class=\"sourcecode\">" << CPPstring << "<p>\n";
	htmlFile << "\t\t</div>\n";

	//right column
	htmlFile << "\t\t<div id=column2 clss=\"columnContainer\">";

	stringstream suggestions(Suggestions);
	//load suggestions in right column
	if (Suggestions != "")
	{
		while (getline(suggestions, tempString))
		{
			htmlFile << "\t\t\t<p class=\"rule\">" << tempString << "<p>\n";
		}
	}
	else
		htmlFile << "\t\t\t<p class=\"rule\">" << "No Suggestions!" << "<p>\n";

	htmlFile << "\t\t</div>\n" << "\t</body>\n" << "</html>";
}

void createHTML::makeCSSfile() 
{
	ofstream  cssFile(htmlLocation + "//htmlStyle.css");

	cssFile << ".columnContainer{\n" << "\tfloat: left;\n" << "\twidth: 50%" << "}\n\n";
	cssFile << ".sourcecode{\n" 
		<< "\tposition: relative;"
		<< "\tborder-radius: 25px;\n"
		<< "\tborder: 2px solid #227db3;\n"
		<< "\tpadding: 20px;\n"
		<< "\twidth: 400px;\n"
		<< "\theight: 500px;\n"
		<< "\tmargin: 0;\n"
		<< "\tfont-family: \"Lucida Console\";\n"
		<< "}\n\n";

	cssFile << ".rule{\n"
		<< "\tposition: relative;"
		<< "\tborder-radius: 25px;\n"
		<< "\tborder: 2px solid #227db3;\n"
		<< "\tpadding: 20px;\n"
		<< "\twidth: 400px;\n"
		<< "\theight: 50px;\n"
		<< "\tmargin: 0;\n"
		<< "\tfont-family: \"Lucida Console\";\n"
		<< "}\n\n";
}

#endif // !CREATE_HTML_FILE