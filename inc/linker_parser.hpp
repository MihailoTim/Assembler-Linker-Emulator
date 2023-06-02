#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef _PARSER_H
#define _PARSER_H

class Parser{
public:
	enum CurrentlyReading{NOTHING, SECTIONHEADER, SECTIONCONTENT, SYMBOLTABLE, RELOCATIONS};
	static CurrentlyReading status;
	static string currentSection;

	static void parseFile(char* fileIn);

	static void getStatus(string line);

	static void handleSectionLine(string line);

	static void handleSectionContent(string line);

	static void handleSymbolTableLine(string line);
};

#endif