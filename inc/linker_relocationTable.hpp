#include <iostream>
#include <map>
#include <vector>
using namespace std;

#ifndef _RELOCATION_TABLE_H
#define _RELOCATION_TABLE_H

class RelocationTable{
public:
	struct RelocationTableLine{
		size_t location;
		string type;
		string symbol;
		int addend;
		string section;
		size_t sectionBase;
		RelocationTableLine(size_t l, string t, string s, int a, string sctn, size_t sb): location(l), type(t), symbol(s), addend(a), section(sctn), sectionBase(sb){}
	};

	static vector<RelocationTableLine*> outputReloTable;

	static void handleReloLine(size_t location, string type, string symbol, size_t addend);

	static void addReloLineToOutput(size_t location, string type, string symbol, size_t addend, string section, size_t base);

	static void resolveRelocations();

	static void printRelocationsToOutput(ostream &out, string section);
};

#endif