#include <iostream>
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
	static void handleReloLine(size_t location, string type, string symbol, size_t addend);
};

#endif