#include <iostream>
#include <vector>
#include <string>
#include "../inc/assembler_symbolTable.hpp"

using namespace std;

#ifndef _RELOCATION_TABLE_H
#define _RELOCATION_TABLE_H


class RelocationTable{
public:
	friend class SecondPass;
	friend class LiteralPool;
private:
	struct RelocationTableLine{
		enum RelocationType{R_32, R_PC32};
		size_t offset;
		RelocationType type;
		size_t referencedSymbol;
		string relocatedSymbol;
		long addend;
		RelocationTableLine(size_t o, RelocationType rt, size_t s, long a, string reloSymbol): offset(o), type(rt), referencedSymbol(s), addend(a), relocatedSymbol(reloSymbol){}

		void printReloTableLine(){
			cout << offset << " " << (type == R_32 ? "R_32" : "R_PC32") <<" " << referencedSymbol << " " << addend;
		}
	};

	using RelocationType = RelocationTable::RelocationTableLine::RelocationType;

	RelocationTable(string sctn) : count(0), symbolTable(SymbolTable::getInstance()), section(sctn){
		reloTable.clear();
	}

	RelocationTableLine* handleNewReloLine(size_t offset, RelocationType type, string symbol);

	vector<string> getContent();

	SymbolTable::SymbolTableLine getSymbolToReference(string symbol);
	
	SymbolTable &symbolTable;
	map<size_t, RelocationTableLine*> reloTable;
	string section;
	size_t count;
};

#endif