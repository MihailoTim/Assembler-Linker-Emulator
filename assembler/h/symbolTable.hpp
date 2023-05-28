#include <iostream>
#include <vector>
#include <map>
#include "../h/exceptions.hpp"
using namespace std;

#ifndef _SYMBOL_TABLE_HPP
#define _SYMBOL_TABLE_HPP

class SymbolTable{
public:
	enum SymbolSection{GLOBAL=-1, UNDEFINED=0};
	enum SymbolBind{LOC, GLOB, UNBOUND};
	enum SymbolType{NOTYPE, SCTN, OBJ, FUNC, UND};

	static SymbolTable& getInstance(){
		static SymbolTable instance;
		return instance;
	}

	void handleGlobalDirective(string symbol);

	void handleExternDirective(string symbol);

	void handleSectionDirective(string symbol, size_t locationCounter);

	void handleEndDirective(size_t locationCounter);

	void handleEquDirective(string symbol, size_t locationCounter);

	void handleLabel(string symbol, size_t locationCounter);

	void handleSymbolReference(string symbol, size_t locationCounter);

	void printSymbolTable();

	void printSectionTable();

	void printSection(string section);

	void printAllSections();

	friend class SecondPass;
	friend class RelocationTable;
private:
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
	struct SectionTableLine{
		size_t base;
		size_t length;
		string name;
		string content;
		vector<string> reloTable;
		size_t symTabId;
		SectionTableLine(){}
		SectionTableLine(size_t b, size_t l, string n, size_t stid): base(b), length(l), name(n), symTabId(stid){}
	};

	map<string, SymbolTableLine> symbolTable;
	map<string, SectionTableLine> sectionTable;
	map<size_t, string> symbolLookupTable;
	size_t count;	
	string currentSection;

	SymbolTable(SymbolTable const&);
	void operator=(SymbolTable const&);


	SymbolTableLine* handleNewSymbol(size_t n, size_t v, size_t s, SymbolType st,  SymbolBind sb, int ind, string sname);

	SymbolTable(){
		count=0;
		SymbolTableLine *stline = handleNewSymbol(count++,0,0,SymbolType::NOTYPE, SymbolBind::LOC,SymbolSection::UNDEFINED, "");
		symbolTable.insert(make_pair("", *stline));
		SectionTableLine secline = *new SectionTableLine(0, 0, "", 0);
		sectionTable.insert(make_pair("", secline));
		currentSection = "";
	}
};

#endif