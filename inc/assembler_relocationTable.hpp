#include <iostream>
#include <vector>
#include <string>
#include "../inc/assembler_symbolTable.hpp"
#include "../inc/assembler_relocationTableLine.hpp"

using namespace std;

#ifndef _RELOCATION_TABLE_H
#define _RELOCATION_TABLE_H


class RelocationTable{
public:
	friend class SecondPass;
	friend class LiteralPool;
	friend class EquTable;
	friend class SymbolTable;
private:

	using RelocationType = RelocationTableLine::RelocationType;


	RelocationTable(string sctn) : count(0), symbolTable(SymbolTable::getInstance()), section(sctn){
		reloTable.clear();
	}

	RelocationTableLine* handleNewReloLine(size_t offset, RelocationType type, string symbol);

	vector<RelocationTableLine*> getContent();

	SymbolTable::SymbolTableLine getSymbolToReference(string symbol);
	
	SymbolTable &symbolTable;
	map<size_t, RelocationTableLine*> reloTable;
	string section;
	size_t count;
};

#endif