#include "../h/symbolTable.hpp"

void SymbolTable::handleGlobalDirective(string symbol){
	if(symbolTable.count(symbol)){
		SymbolTableLine &stline = symbolTable[symbol];
		stline.bind = SymbolBind::GLOB;
	}
	else{
		SymbolTableLine *stline = new SymbolTableLine(this->count++, 0, 0,SymbolType::NOTYPE, SymbolBind::GLOB, SymbolSection::GLOBAL, symbol);
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::handleExternDirective(string symbol){
	if(symbolTable.count(symbol)){
		throw new Exception("Multiple definitions of symbol " + symbol);
	}
	else{
		SymbolTableLine *stline = new SymbolTableLine(this->count++, 0, 0,SymbolType::NOTYPE, SymbolBind::GLOB, SymbolSection::UNDEFINED, symbol);
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::handleSectionDirective(string symbol, size_t locationCounter){
	if(sectionTable.count(symbol)>0){
		throw new Exception("Section "+ symbol + " has already been defined");
	}
	else{
		SectionTableLine &sctLine = sectionTable[currentSection];
		sctLine.length = locationCounter;
		currentSection = symbol;
		SectionTableLine newSctLine = *new SectionTableLine(0, 0, symbol);
		sectionTable.insert(make_pair(symbol, newSctLine));
		int num = this->count++;
		SymbolTableLine stline = *new SymbolTableLine(num, 0, 0,SymbolType::SCTN, SymbolBind::LOC, num, symbol);
		symbolTable.insert(make_pair(symbol, stline));
	}
}

void SymbolTable::handleEquDirective(string symbol, size_t locationCounter){
	SymbolTableLine section = symbolTable[currentSection];
	if(symbolTable.count(symbol)>0){
		SymbolTableLine &stline = symbolTable[symbol];
		if(stline.ndx == SymbolSection::GLOBAL){
			stline.ndx = section.num;
		}
		else{
			throw new Exception("Symbol "+ symbol + " has already been defined");
		}
	}
	else{
		SymbolTableLine stline = *new SymbolTableLine(this->count++, 0, 0,SymbolType::NOTYPE, SymbolBind::LOC, section.num, symbol);
		symbolTable.insert(make_pair(symbol, stline));
	}
}

void SymbolTable::handleEndDirective(size_t locationCounter){
	SectionTableLine &sctLine = sectionTable[currentSection];
	sctLine.length = locationCounter;
	currentSection = "";
	printSymbolTable();
	printSectionTable();
}

void SymbolTable::handleLabel(string symbol, size_t locationCounter){
	SymbolTableLine section = symbolTable[currentSection];
	if(symbolTable.count(symbol)>0){
		SymbolTableLine &stline = symbolTable[symbol];
		if(stline.ndx == SymbolSection::GLOBAL){
			stline.value = locationCounter;
			stline.ndx = section.num;
		}
		else{
			throw new Exception("Symbol "+ symbol + " has already been defined");
		}
	}
	else{
		SymbolTableLine stline = *new SymbolTableLine(this->count++, locationCounter, 0,SymbolType::NOTYPE, SymbolBind::LOC, section.num, symbol);
		symbolTable.insert(make_pair(symbol, stline));
	}
}

void SymbolTable::handleSymbolReference(string symbol, size_t locationCounter){
	SymbolTableLine section = symbolTable[currentSection];
	if(symbolTable.count(symbol) == 0){
		SymbolTableLine stline = *new SymbolTableLine(this->count++, 0, 0,SymbolType::NOTYPE, SymbolBind::UNBOUND, section.num, symbol);
		symbolTable.insert(make_pair(symbol, stline));
	}
}

void SymbolTable::printSymbolTable(){
	for(auto it = symbolTable.begin(); it!=symbolTable.end();it++){
		SymbolTableLine stline = it->second;
		cout<<stline.num<<" "<<stline.value<<" "<<stline.size<<" "<<stline.type<<" "<<stline.bind<<" "<<stline.ndx<<" "<<stline.name<<endl;
	}
}

void SymbolTable::printSectionTable(){
	for(auto it = sectionTable.begin(); it!=sectionTable.end();it++){
		SectionTableLine stline = it->second;
		cout<<stline.base<<" "<<stline.length<<" "<<stline.name<<endl;
	}
}