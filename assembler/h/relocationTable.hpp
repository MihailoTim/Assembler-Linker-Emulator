#include <iostream>
#include <vector>
#include <string>
#include "../h/symbolTable.hpp"

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
		size_t symbol;
		long addend;
		RelocationTableLine(size_t o, RelocationType rt, size_t s, long a): offset(o), type(rt), symbol(s), addend(a){}

		void printReloTableLine(){
			cout << offset << " " << (type == R_32 ? "R_32" : "R_PC32") <<" " << symbol << " " << addend;
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