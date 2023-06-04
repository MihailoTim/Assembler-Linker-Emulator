#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "../inc/linker_symbolTable.hpp"
#include "../inc/linker_relocationTable.hpp"

using namespace std;

#ifndef _PARSER_H
#define _PARSER_H

class Parser{
public:
	enum CurrentlyReading{NOTHING, SECTIONHEADER, SECTIONCONTENT, SYMBOLTABLE, RELOCATIONS};
	static CurrentlyReading status;
	static string currentSection;
	static string sectionContent;
	static vector<SymbolTable::SymbolTableLine*> localSymbolTable;
	static map<string, string> localSectionContent;
	static vector<RelocationTable::RelocationTableLine*> localReloTable;
	static size_t currentBase;

	static void parseFile(char* fileIn);

	static void getStatus(string line);

	static void handleSectionLine(string line);

	static void handleSectionContent(string line);

	static void handleSymbolTableLine(string line);

	static void handleRelocationLine(string line);

	static string getByte(uint8_t byte);

	static string get4Bytes(int bytes);
};

#endif