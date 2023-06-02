#include "../inc/linker_symbolTable.hpp"
#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_exceptions.hpp"

map<string, SymbolTable::SymbolTableLine*> SymbolTable::symbolTable;
map<size_t, string> SymbolTable::symbolLookupTable;

void SymbolTable::insertNewSymbol(size_t num, size_t value, size_t size, SymbolType type, SymbolBind bind, int ndx, string name, string section){
	// cout<<name<<" "<<value<<endl;
	SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[section];
	if(bind == SymbolBind::LOC)
		return;
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
			stline->value = sctnline->base + sctnline->content.size()/2 + value;
			return;
		}
		throw new Exception("Multiple definitions of same symbol");
	}
	size_t n = symbolTable.size();
	// cout<<"SECTION: "<<sctnline->name<<endl;
	// cout<<"SECTION BASE:"<<sctnline->base<<endl;
	// cout<<"SECTION SIZE:"<<sctnline->content.size()/2<<endl;
	size_t val = sctnline->base + sctnline->content.size()/2 + value;
	int index = ndx > 0 ? symbolTable[section]->num : ndx;
	SymbolTableLine *stline = new SymbolTableLine(n, val, 0, type, bind, index, name);
	symbolTable.insert(make_pair(name, stline));
	symbolLookupTable.insert(make_pair(n, name));
}

void SymbolTable::printSymbolTable(){
	cout<<"#symbtab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		SymbolTableLine *stline = symbolTable[it->second];
		cout<<stline->num<<" "<<stline->value<<" "<<stline->size<<" "<<stline->type<<" "<<stline->bind<<" "<<stline->ndx<<" "<<stline->name<<" ";
		cout<<endl;
	}
	cout<<"#lookuptab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		cout<<it->first<<" " <<it->second<<endl;
	}
}
