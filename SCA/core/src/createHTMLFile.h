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
	string sourceTitle = "<p class=\"titleText\">Source Code</p><br/>";
	string suggestionsTitle = "<p class=\"titleText\">Suggestions</p><br/>";
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
  
  	htmlFile << "\t\t<h1>\n" << "\t\t\t" << fileName << ".cpp\n" << "\t\t</h1>";
	htmlFile << "\t\t<h2>\n" << "\t\t\t<i>Output from SCA (v1.0) - https://github.com/alihaider1264/SCA.git</i>\n" << "\t\t</h2>";
  
	//left column
	htmlFile << "<div id=\"column1\" class=\"columnContainer\">\n";
	


	ifstream cppFile(cppFileLocation);
	string CPPstring = "<strong>" + fileName + ".cpp </strong> <br/><br/>";
	string tempString = "";
	int lineNum = 1;

	while (getline(cppFile, tempString))//load the cpp file into CPP string
	{

		// remove all less than and greater than symbols (<,>) from the tempString
		// replace with &lt; and &gt; so they can be written to the html file.
		size_t ltFound = tempString.find("<");
		size_t gtFound = tempString.find(">");

		while (ltFound != string::npos || gtFound != string::npos) {
			if (ltFound != string::npos) 
				tempString = tempString.substr(0, ltFound) + "&lt;" + tempString.substr(ltFound + 1, tempString.size() - ltFound);
			
			gtFound = tempString.find(">");

			if (gtFound != string::npos)
				tempString = tempString.substr(0, gtFound) + "&gt;" + tempString.substr(gtFound + 1, tempString.size() - gtFound);

			ltFound = tempString.find("<"); 
		}


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

	htmlFile << "</div>";//close left column

	//right column
	htmlFile << "<div id=\"column2\" class=\"columnContainer\">";
	htmlFile << suggestionsTitle;

	//clear tempString
	tempString = "";
  
	htmlFile << "\t\t\t<div class=\"ruleContainer\">";

	// Check if there are suggestions produced by component classes
	bool hasWhileLoopSuggestions = false;
	bool hasForLoopSuggestions = false;
	bool hasIfSuggestions = false;
	bool hasSwitchSuggestions = false;
	bool hasSuggestions = false;

	for (While_Loop* wl : whileLoopComponents) {
		if (!wl->getCorrectComponent()) {
			hasWhileLoopSuggestions = true;
			break;
		}
	}
	for (ForLoop* fl : forLoopComponents) {
		if (!fl->getCorrectComponent()) {
			hasForLoopSuggestions = true;
			break;
		}
	}
	for (If* iff : ifComponents) {
		if (!iff->getCorrectComponent()) {
			hasIfSuggestions = true;
			break;
		}
	}
	for (Switch* sw : switchComponents) {
		if (!sw->getCorrectComponent()) {
			hasSwitchSuggestions = true;
			break;
		}
	}

	stringstream suggestions(Suggestions);
	//load suggestions in right column
	if (Suggestions != "")
	{
		while (getline(suggestions, tempString))
		{

			htmlFile << "<p class=\"rule\">" << tempString << "</p>";
		}
		hasSuggestions = true;
	}
	if (hasWhileLoopSuggestions) {
		for (While_Loop* wl : whileLoopComponents) {
			if (!wl->getCorrectComponent()) {
				for (string s : wl->getCodeSmells()) {
					htmlFile << "<p class=\"rule\"><strong>" << wl->getLoopType() << " loop on line " << to_string(wl->getBegLineNum());
					htmlFile << ":</strong> " << s << "</p>";
				}
			}
		}
	}
	if (hasForLoopSuggestions) {
		for (ForLoop* fl : forLoopComponents) {
			if (!fl->getCorrectComponent()) {
				for (string s : fl->getCodeSmells()) {
					htmlFile << "<p class=\"rule\"><strong>For Loop on line " << to_string(fl->getFirstStartLine());
					htmlFile << ":</strong> " << s << "</p>";
				}
			}
		}
	}
	if (hasIfSuggestions) {
		for (If* iff : ifComponents) {
			if (!iff->getCorrectComponent()) {
				for (string s : iff->getCodeSmells()) {
					htmlFile << "<p class=\"rule\"><strong>If Statement: ";
					htmlFile << ":</strong> " << s << "</p>";
				}
			}
		}
	}
	if (hasSwitchSuggestions) {
		for (Switch* sw : switchComponents) {
			if (!sw->getCorrectComponent()) {
				for (string s : sw->getCodeSmells()) {
					htmlFile << "<p class=\"rule\">" << s << "</p>";
				}
			}
		}
	}
	if (!hasSuggestions && !hasWhileLoopSuggestions && !hasForLoopSuggestions && !hasIfSuggestions && !hasSwitchSuggestions) {
		htmlFile << "<p class=\"rule\">" << "No Suggestions!" << "</p>";
	}

	cout << Suggestions << endl;
	htmlFile << "</div>";//close ruleContainer
	htmlFile << "</div>";//close left column

	//components
	htmlFile << "<div class=\"components\"><p class=\"titleText\">Components</p><br/>";

	for (int i = 0; i < whileLoopComponents.size(); i++)
	{
		if (whileLoopComponents[i]->getCorrectComponent())
		{
			htmlFile << "<div class=\"correctComponent\">\n";
			htmlFile << whileLoopComponents[i]->getComponent() << "<br />";
			htmlFile << "</div>";
		}
		else
		{
			htmlFile << "<div class=\"wrongComponent\">";
			htmlFile <<  whileLoopComponents[i]->getComponent() << "<br />";
			//for (string s : whileLoopComponents[i]->getCodeSmells()) {
			//	htmlFile << s;
			//}
			htmlFile << "</div>";
		}
	}

	for (int i = 0; i < forLoopComponents.size(); i++)
	{
		if (forLoopComponents[i]->getCorrectComponent())
		{
			htmlFile << "<div class=\"correctComponent\">";
			htmlFile <<  forLoopComponents[i]->getComponent() << "<br />";
			htmlFile << "</div>";
		}
		else
		{
			htmlFile << "<div class=\"wrongComponent\">";
			htmlFile <<  forLoopComponents[i]->getComponent() << "<br />";
			//for (string s : forLoopComponents[i]->getCodeSmells()) {
			//	htmlFile << s;
			//}
			htmlFile << "</div>";
		}
	}

	for (int i = 0; i < ifComponents.size(); i++)
	{
		if (ifComponents[i]->getCorrectComponent())
		{
			htmlFile << "\t\t\t<div class=\"correctComponent\">\n";
			htmlFile << "\t\t\t\t" << ifComponents[i]->getComponent() << "<br />";
			htmlFile << "\t\t\t</div>\n";
		}
		else
		{
			htmlFile << "\t\t\t<div class=\"wrongComponent\">\n";
			htmlFile << "\t\t\t\t" << ifComponents[i]->getComponent() << "<br />";
			//for (string s : ifComponents[i]->getCodeSmells()) {
			//	htmlFile << s;
			//}
			htmlFile << "\t\t\t</div>\n";
		}
	}

	for (int i = 0; i < switchComponents.size(); i++)
	{
		if (switchComponents[i]->getCorrectComponent())
		{
			htmlFile << "\t\t\t<div class=\"correctComponent\">\n";
			htmlFile << "\t\t\t\t" << switchComponents[i]->getComponent() << "<br />";
			htmlFile << "\t\t\t</div>\n";
		}
		else
		{
			htmlFile << "\t\t\t<div class=\"wrongComponent\">\n";
			htmlFile << "\t\t\t\t" << switchComponents[i]->getComponent() << "<br />";
			//for (string s : switchComponents[i]->getCodeSmells()) {
			//    htmlFile << s;
			//}
			htmlFile << "\t\t\t</div>\n";
		}
	}

	htmlFile << "</div></div></body></html>";//close components then entire page

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