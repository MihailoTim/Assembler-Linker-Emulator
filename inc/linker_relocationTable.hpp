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
		size_t addend;
		string section;
		RelocationTableLine(size_t l, string t, string s, size_t a, string sctn): location(l), type(t), symbol(s), addend(a), section(sctn){}
	};
	static void handleReloLine(size_t location, string type, string symbol, size_t addend);
};

#endif