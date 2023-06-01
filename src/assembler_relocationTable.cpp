#include "../inc/assembler_relocationTable.hpp"

RelocationTable::RelocationTableLine* RelocationTable::handleNewReloLine(size_t offset, RelocationType type, string symbol){
	SymbolTable::SymbolTableLine symbolToRelo = symbolTable.symbolTable[symbol];
	SymbolTable::SymbolTableLine symbolToRef = getSymbolToReference(symbol);
	long addend = symbolToRelo.value - symbolToRef.value  - (type == RelocationType::R_PC32 ? 4 : 0);

	RelocationTableLine* line = new RelocationTableLine(offset, type, symbolToRef.num, addend, symbolToRelo.name);

	reloTable.insert(make_pair(offset, line));

	return line;
}

SymbolTable::SymbolTableLine RelocationTable::getSymbolToReference(string symbol){
	SymbolTable::SymbolTableLine stline = symbolTable.symbolTable[symbol];
	while(stline.type != SymbolTable::SymbolType::SCTN && stline.bind != SymbolTable::SymbolBind::GLOB && stline.num != 0){
		string nextSymbol = symbolTable.symbolLookupTable[stline.ndx];
		stline = symbolTable.symbolTable[nextSymbol];
	}
	return stline;
}

vector<string> RelocationTable::getContent(){
	vector<string> result;
	for(auto it = reloTable.begin();it!=reloTable.end();it++){
		RelocationTableLine *line = it->second;
		result.push_back(to_string(line->offset) + " " + (line->type == RelocationType::R_32 ? "R_32" : "R_PC32") + " "+ to_string(line->referencedSymbol) + " "+ to_string(line->addend));
	}
	return result;
}