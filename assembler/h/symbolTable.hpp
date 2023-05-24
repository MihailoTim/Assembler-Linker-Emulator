#include <iostream>
#include <vector>
#include <map>
#include "../h/exceptions.hpp"
using namespace std;

class SymbolTable{
public:
	static SymbolTable& getInstance(){
		static SymbolTable instance;
		return instance;
	}

	void handleGlobalDirective(string symbol);

	void handleExternDirective(string symbol);

	void handleSectionDirective(string symbol, size_t locationCounter);

	void handleEndDirective(size_t locationCounter);

	void handleLabel(string symbol, size_t locationCounter);

	void printSymbolTable();

	void printSectionTable();
private:
	enum SymbolSection{GLOBAL=-1, UNDEFINED=0};
	enum SymbolBind{LOC, GLOB, UNBOUND};
	enum SymbolType{NOTYPE, SCTN, OBJ, FUNC, UND};
	struct SymbolTableLine{
		size_t num;
		size_t value;
		size_t size;
		SymbolType type;
		SymbolBind bind;
		int ndx;
		string name;

		SymbolTableLine(){}
		SymbolTableLine(size_t n, size_t v, size_t s, SymbolType st,  SymbolBind sb, int ind, string sname) : num(n), value(v), ndx(ind), bind(sb), type(st), name(sname), size(s){}
	};
	struct SectionTableLine{
		size_t base;
		size_t length;
		string name;
		vector<string> content;
		SectionTableLine(){}
		SectionTableLine(size_t b, size_t l, string n): base(b), length(l), name(n){}
	};

	map<string, SymbolTableLine> symbolTable;
	map<string, SectionTableLine> sectionTable;
	size_t count;	
	string currentSection;

	SymbolTable(){
		count=0;
		SymbolTableLine stline = *new SymbolTableLine(count++,0,0,SymbolType::NOTYPE, SymbolBind::LOC,SymbolSection::UNDEFINED, "");
		symbolTable.insert(make_pair("", stline));
		SectionTableLine secline = *new SectionTableLine(0, 0, "");
		sectionTable.insert(make_pair("", secline));
		currentSection = "";
	}
	SymbolTable(SymbolTable const&);
	void operator=(SymbolTable const&);
};