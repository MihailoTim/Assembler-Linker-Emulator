#include "../inc/linker_symbolTable.hpp"
#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_exceptions.hpp"
#include <fstream>

map<string, SymbolTable::SymbolTableLine*> SymbolTable::symbolTable;
map<int, string> SymbolTable::symbolLookupTable;

void SymbolTable::insertNewSymbol(size_t num, size_t value, size_t size, SymbolType type, SymbolBind bind, int ndx, string name, string section){
	if(type == SymbolType::EQU_SYMBOL || type==SymbolType::EQU_LITERAL){
		insertNewEquSymbol(num, value, size, type, bind, ndx, name, section);
		return;
	}
	SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[section];
	if(bind == SymbolBind::LOC)
		return;
	// cout<<"Name: "<<name<<" Type: "<<type<<endl;
	if(symbolTable.count(name)){
		SymbolTableLine *stline = symbolTable[name];
		if(stline->type == SymbolType::SCTN && type == SymbolType::SCTN)
			return;
		if(stline->ndx == SymbolSection::EXTERN && ndx == SymbolSection::EXTERN)
			return;
		if(stline->ndx != SymbolSection::EXTERN && ndx == SymbolSection::EXTERN){			
			return;
		}
		if(stline->ndx == SymbolSection::EXTERN && ndx != SymbolSection::EXTERN){
			stline->ndx = symbolTable[section]->num;
			// cout<<"SECTION: "<<sctnline->name<<endl;
			// cout<<"SECTION BASE:"<<sctnline->base<<endl;
			// cout<<"SECTION SIZE:"<<sctnline->content.size()/2<<endl;
			stline->offset = sctnline->content.size()/2 + value;
			return;
		}
		throw new Exception("Multiple definitions of same symbol "+name);
	}
	size_t n = symbolTable.size();
	// cout<<"SECTION: "<<sctnline->name<<endl;
	// cout<<"SECTION BASE:"<<sctnline->base<<endl;
	// cout<<"SECTION SIZE:"<<sctnline->content.size()/2<<endl;
	size_t val = sctnline->content.size()/2 + value;
	int index = ndx > 0 ? symbolTable[section]->num : ndx;
	SymbolTableLine *stline = new SymbolTableLine(n, val, 0, type, bind, index, name);
	symbolTable.insert(make_pair(name, stline));
	symbolLookupTable.insert(make_pair(n, name));
}

void SymbolTable::insertNewEquSymbol(size_t num, size_t value, size_t size, SymbolType type, SymbolBind bind, int ndx, string name, string section){
	// cout<<"Name: "<<name<<" Type: "<<type<<endl;
	if(symbolTable.count(name)){
		SymbolTableLine *stline = symbolTable[name];
		if(stline->type == SymbolType::SCTN && type == SymbolType::SCTN)
			return;
		if(stline->ndx == SymbolSection::EXTERN && ndx == SymbolSection::EXTERN)
			return;
		if(stline->ndx != SymbolSection::EXTERN && ndx == SymbolSection::EXTERN){			
			return;
		}
		if(stline->ndx == SymbolSection::EXTERN && ndx != SymbolSection::EXTERN){
			stline->ndx = symbolTable[section]->num;
			stline->offset = value;
			stline->type = type;
			return;
		}
		throw new Exception("Multiple definitions of same symbol "+name);
	}
	size_t n = symbolTable.size();
	int index = ndx > 0 ? symbolTable[section]->num : ndx;
	SymbolTableLine *stline = new SymbolTableLine(n, value, 0, type, bind, index, name);
	symbolTable.insert(make_pair(name, stline));
	symbolLookupTable.insert(make_pair(n, name));
}

void SymbolTable::printSymbolTable(){
	cout<<"#symtab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		SymbolTableLine *stline = symbolTable[it->second];
		cout<<stline->num<<" "<<stline->offset<<" "<<stline->virtualAddress<<" "<<stline->size<<" "<<stline->type<<" "<<stline->bind<<" "<<stline->ndx<<" "<<stline->name<<endl;
	}
	cout<<"#lookuptab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		cout<<dec<<it->first<<" " <<it->second<<endl;
	}
}

void SymbolTable::updateSymbolVirtualAddresses(){
	for(auto it = SymbolTable::symbolTable.begin(); it != SymbolTable::symbolTable.end(); it++){
		if(it->second->ndx <0 || it->second->type == SymbolType::EQU_SYMBOL || it->second->type == SymbolType::EQU_LITERAL)
			continue;
		string section = SymbolTable::symbolLookupTable[it->second->ndx];
		SectionTable::SectionTableLine *sctn = SectionTable::sectionTable[section];
		if(it->second->type != SymbolTable::SymbolType::SCTN)
			it->second->virtualAddress = sctn->base + it->second->offset;
		else
			it->second->offset = 0;
	}
}

void SymbolTable::updateEquSymbolVirtualAddresses(){
	for(auto it = SymbolTable::symbolTable.begin(); it != SymbolTable::symbolTable.end(); it++){
		if(it->second->type == SymbolType::EQU_SYMBOL){
			string refSym = SymbolTable::symbolLookupTable[it->second->ndx];
			SymbolTable::SymbolTableLine* refLine = SymbolTable::symbolTable[refSym];
			size_t addend = 0;
			while(refLine->type == EQU_SYMBOL){
				addend+=refLine->offset;
				refSym = SymbolTable::symbolLookupTable[refLine->ndx];
				if(refSym == it->second->name){
					throw new Exception("Circular dependency while resovling .equ");
				}
				refLine = SymbolTable::symbolTable[refSym];
			}
			addend += refLine->virtualAddress;
			it->second->virtualAddress = it->second->offset + addend;;
		}
		else
			if(it->second->type == SymbolType::EQU_LITERAL){
				it->second->virtualAddress = it->second->offset;
			}
	}
}

bool SymbolTable::checkHexCompatible(){
	for(auto it = SymbolTable::symbolTable.begin(); it!= SymbolTable::symbolTable.end(); it++){
		if(it->second->ndx < 0 && it->second->type != SymbolType::EQU_LITERAL && it->second->type != SymbolType::EQU_SYMBOL)
			return false;
	}
	return true;
}

void SymbolTable::printSymbolTableToOutput(ofstream &out){
	out<<"#symtab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		SymbolTableLine *stline = symbolTable[it->second];
		out<<stline->num<<" "<<stline->offset<<" "<<stline->size<<" "<<stline->type<<" "<<stline->bind<<" "<<stline->ndx<<" "<<stline->name<<endl;
	}
}