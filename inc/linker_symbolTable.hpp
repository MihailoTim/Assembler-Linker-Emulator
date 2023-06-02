#include <iostream>
#include <map>
#include <string>
using namespace std;

#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

class SymbolTable{
public:
	enum SymbolSection{EXTERN=-2, GLOBAL=-1, UNDEFINED=0};
	enum SymbolBind{LOC, GLOB, UNBOUND};
	enum SymbolType{NOTYPE, SCTN, OBJ, FUNC, UND};
	enum ReferenceLocation{DIRECT, INDIRECT};

	struct SymbolTableLine{
		size_t num;
		size_t value;
		size_t size;
		SymbolType type;
		SymbolBind bind;
		int ndx;
		string name;
		bool global;
		SymbolTableLine(){}
		SymbolTableLine(size_t n, size_t v, size_t s, SymbolType st,  SymbolBind sb, int ind, string sname) : num(n), value(v), ndx(ind), bind(sb), type(st), name(sname), size(s), global(false){}
	};

	static map<string, SymbolTableLine*> symbolTable;
	static map<size_t, string> symbolLookupTable;

	static void insertNewSymbol(size_t num, size_t value, size_t size, SymbolType type, SymbolBind bind, int ndx, string name, string section);

	static void printSymbolTable();
};

#endif