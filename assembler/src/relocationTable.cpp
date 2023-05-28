#include "../h/relocationTable.hpp"

string RelocationTable::handleNewReloLine(size_t offset, RelocationType type, string symbol){
	SymbolTable::SymbolTableLine symbolToRelo = symbolTable.symbolTable[symbol];
	SymbolTable::SymbolTableLine symbolToRef = getSymbolToReference(symbol);
	long addend = symbolToRelo.value - symbolToRef.value  - (type == RelocationType::R_PC32 ? 4 : 0);

	string res = "";

	res = to_string(offset) + " " + (type == RelocationType::R_32 ? "R_32" : "R_PC32") + " " +  symbolToRef.name + " " + to_string(addend);

	return res;
}

SymbolTable::SymbolTableLine RelocationTable::getSymbolToReference(string symbol){
	SymbolTable::SymbolTableLine stline = symbolTable.symbolTable[symbol];
	while(stline.type != SymbolTable::SymbolType::SCTN && stline.bind != SymbolTable::SymbolBind::GLOB && stline.num != 0){
		string nextSymbol = symbolTable.symbolLookupTable[stline.ndx];
		stline = symbolTable.symbolTable[nextSymbol];
	}
	return stline;
}