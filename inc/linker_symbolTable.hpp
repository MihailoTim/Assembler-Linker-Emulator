#include <iostream>
#include <map>
#include <string>
using namespace std;

class SymbolTable{
public:
	enum SymbolSection{GLOBAL=-1, UNDEFINED=0};
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

	map<string, SymbolTableLine*> SymbolTable;
	map<size_t, string> symbolLookupTable;
};