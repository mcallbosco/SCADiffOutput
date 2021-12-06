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
	vector<While_Loop*> whileLoopComponents;
	vector<ForLoop*> forLoopComponents;
	vector<If*> ifComponents;
	vector<Switch*> switchComponents;
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

	//get all component node pointers
	void setwhileLoopComponents(vector<While_Loop*> whileLoopComps);
	void setForLoopComponents(vector<ForLoop*> forLoopComps);
	void setIfComponents(vector<If*> ifComps);
	void setSwitchComponents(vector<Switch*> switchComps);
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

	//start of file and header
	htmlFile << "<!DOCTYPE html>\n" << "<html>\n" << "\t<head>\n";

	//css link
	htmlFile << "\t\t<link rel=\"stylesheet\" href=\"htmlStyle.css\"\n>";

	htmlFile << "\t\t<h1>\n" << "\t\t\tcppNameHere.cpp\n" << "\t\t</h1>";
	htmlFile << "\t\t<h2>\n" << "\t\t\t<i>Output from SCA (v1.0) - https://github.com/alihaider1264/SCA.git</i>\n" << "\t\t</h2>";
	
	htmlFile << "\t</head>\n" << "\t<body>\n";

	//left column
	htmlFile << "\t\t<div id=column1 class=\"columnContainer\"\n>";
	
	htmlFile << "\t\t\t<div class=\"sourcecode\">\n";

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
	htmlFile << "\t\t\t\t" << cppFile + "\n" << "\t\t\t</div>\n"
	htmlFile << "\t\t\t</div>\n";
	htmlFile << "\t\t</div>\n";

	//right column
	htmlFile << "\t\t<div id=column2 clss=\"columnContainer\">\n";
	htmlFile << "\t\t\t<div clas = \"ruleContainer\"\n";

	stringstream suggestions(Suggestions);
	//load suggestions in right column
	if (Suggestions != "")
	{
		while (getline(suggestions, tempString))
		{
			htmlFile << "\t\t\t\t<div class=\"rule\">" 
				<< "\t\t\t\t\t" << tempString 
				<< "\t\t\t\t</div>\n";
		}
	}
	else
		htmlFile << "\t\t\t<p class=\"rule\">" << "No Suggestions!" << "<p>\n";

	htmlFile << "\t\t\t</div>\n";
	htmlFile << "\t\t</div>\n";

	//components
	htmlFile << "\t\t<div class = \"components\">";
	
	//htmlFile << "\t\t\tComponents\n";

	for (int i = 0; i < whileLoopComponents.size(); i++)
	{
		if (whileLoopComponents[i]->getCorrectComponent())
		{
			htmlFile << "\t\t\t<div class=\"correctComponent\">\n";
			htmlFile << "\t\t\t\t" << whileLoopComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}
		else
		{
			htmlFile << "\t\t\t<div class=\"wrongComponent\">\n";
			htmlFile << "\t\t\t\t" << whileLoopComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}
	}

	for (int i = 0; i < forLoopComponents.size(); i++)
	{
		if (forLoopComponents[i]->getCorrectComponent())
		{
			htmlFile << "\t\t\t<div class=\"correctComponent\">\n";
			htmlFile << "\t\t\t\t" << forLoopComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}
		else
		{
			htmlFile << "\t\t\t<div class=\"wrongComponent\">\n";
			htmlFile << "\t\t\t\t" << forLoopComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}
	}

	for (int i = 0; i < ifComponents.size(); i++)
	{
		/*if (ifComponents[i]->getCorrectComponent())
		{
			htmlFile << "\t\t\t<div class=\"correctComponent\">\n";
			htmlFile << "\t\t\t\t" << ifComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}
		else
		{
			htmlFile << "\t\t\t<div class=\"wrongComponent\">\n";
			htmlFile << "\t\t\t\t" << ifComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}*/
	}

	for (int i = 0; i < switchComponents.size(); i++)
	{
		/*if (switchComponents[i]->getCorrectComponent())
		{
			htmlFile << "\t\t\t<div class=\"correctComponent\">\n";
			htmlFile << "\t\t\t\t" << switchComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}
		else
		{
			htmlFile << "\t\t\t<div class=\"wrongComponent\">\n";
			htmlFile << "\t\t\t\t" << switchComponents[i]->getComponent() << "\n";
			htmlFile << "\t\t\t</div>\n";
		}*/
	}

	htmlFile << "\t\t</div>\n" << "\t</body>\n" << "</html>";
}

void createHTML::setwhileLoopComponents(vector<While_Loop*> whileLoopComps) {
	whileLoopComponents = whileLoopComps;
}

void createHTML::setForLoopComponents(vector<ForLoop*> forLoopComps) {
	forLoopComponents = forLoopComps;
}

void createHTML::setIfComponents(vector<If*> ifComps) {
	ifComponents = ifComps;
}

void createHTML::setSwitchComponents(vector<Switch*> switchComps) {
	switchComponents = switchComps;
}

#endif // !CREATE_HTML_FILE