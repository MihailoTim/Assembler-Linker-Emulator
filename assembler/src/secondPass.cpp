#include "../h/secondPass.hpp"
#include <map>

void SecondPass::performBetweenPassCheck(){
	map<string, SymbolTable::SymbolTableLine> symTab = symbolTable.symbolTable;
	for(auto stline : symTab){
		if(stline.second.ndx == SymbolTable::SymbolSection::GLOBAL || stline.second.bind == SymbolTable::SymbolBind::UNBOUND){
			throw new Exception("Symbol " + stline.first + " is being used but is not declared");
		}
	}
}

void SecondPass::start(){
	status = RUNNING;
	symbolTable.printSymbolTable();
	symbolTable.printSectionTable();
	performBetweenPassCheck();
}