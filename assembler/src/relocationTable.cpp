#include "../h/relocationTable.hpp"

void RelocationTable::handleNewReloLine(size_t offset, RelocationType type, string symbol){
	SymbolTable::SymbolTableLine symbolToRelo = symbolTable.symbolTable[symbol];
	SymbolTable::SymbolTableLine symbolToRef = getSymbolToReference(symbol);
	long addend = symbolToRelo.value - symbolToRef.value  - (type == RelocationType::R_PC32 ? 4 : 0);

	cout << offset << " " << (type == RelocationType::R_32 ? "R_32" : "R_PC32") <<" "<< symbolToRef.name <<" " << addend<<endl;
}

SymbolTable::SymbolTableLine RelocationTable::getSymbolToReference(string symbol){
	SymbolTable::SymbolTableLine stline = symbolTable.symbolTable[symbol];
	while(stline.type != SymbolTable::SymbolType::SCTN && stline.bind != SymbolTable::SymbolBind::GLOB && stline.num != 0){
		string nextSymbol = symbolTable.symbolLookupTable[stline.ndx];
		stline = symbolTable.symbolTable[nextSymbol];
	}
	return stline;
}