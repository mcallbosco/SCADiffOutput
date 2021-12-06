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
	string sourceTitle = "<p class=\"titleText\">Source Code</p><br/>";
	string suggestionsTitle = "<p class=\"titleText\">Components & Suggestions</p><br/>";
	//create html file
	ofstream htmlFile(htmlLocation);

	//start of file
	htmlFile << "<!DOCTYPE html>\n" << "<html>\n" << "<head>\n";

	//get file name for title
	int start = htmlLocation.find_last_of('/');
	int end = htmlLocation.find_last_of('.');
	string fileName = htmlLocation.substr(start + 1, end - start - 1);
	
	//page title
	htmlFile << "<title>" << fileName << " Results</title>\n"; 

	//css link
	htmlFile << "<link rel=\"stylesheet\" href=\"../../core/src/htmlStyle.css\">\n";

	htmlFile << "</head>\n" << "<body>\n";

	//container for entire page
	htmlFile << "<div class=\"entirePage\">\n";

	//left column
	htmlFile << "<div id=\"column1\" class=\"columnContainer\">\n";

	ifstream cppFile(cppFileLocation);
	string CPPstring = "<strong>" + fileName + ".cpp </strong> <br/><br/>";
	string tempString = "";
	int lineNum = 1;

	while (getline(cppFile, tempString))
	{
		if (lineNum < 10) {
			CPPstring += to_string(lineNum) + "&emsp;" + tempString + "<br/>";
		}
		else if (lineNum > 9 && lineNum < 100) {
			CPPstring += to_string(lineNum) + "&ensp; " + tempString + "<br/>";
		}
		else {
			CPPstring += to_string(lineNum) + "&ensp;" + tempString + "<br/>";
		}
		lineNum++;
	}

	//load soucre code in left column
	htmlFile << sourceTitle << "<p class=\"sourcecode\">" << CPPstring << "</p>\n";
	htmlFile << "</div>\n";

	//right column
	htmlFile << "<div id=column2 class=\"columnContainer\">";
	htmlFile << suggestionsTitle;

	//clear tempString
	tempString = "";

	stringstream suggestions(Suggestions);
	//load suggestions in right column
	if (Suggestions != "")
	{
		while (getline(suggestions, tempString))
		{
			htmlFile << "<p class=\"rule\">" << tempString << "<br/></p>";
		}
	}
	else
		htmlFile << "<p class=\"rule\">" << "No Suggestions!" << "<br/></p>";

	htmlFile << "</div>\n" << "</div>\n" << "</body>\n" << "</html>\n";
}

#endif // !CREATE_HTML_FILE