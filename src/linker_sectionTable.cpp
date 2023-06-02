#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_symbolTable.hpp"

map<string, SectionTable::SectionTableLine*> SectionTable::sectionTable;
size_t SectionTable::totalSize = 0;

void SectionTable::addNewSection(string name, size_t length){
	if(sectionTable.count(name) == 0){
		SectionTableLine *sctnline = new SectionTableLine(name, totalSize);
		totalSize += length;
		sectionTable.insert(make_pair(name, sctnline));
		int ndx = SymbolTable::symbolTable.size();
		SymbolTable::SymbolTableLine *stline = new SymbolTable::SymbolTableLine(ndx, 0, 0, SymbolTable::SymbolType::SCTN, SymbolTable::SymbolBind::LOC, ndx, name);
		SymbolTable::symbolTable.insert(make_pair(name, stline));
		SymbolTable::symbolLookupTable.insert(make_pair(ndx, name));
	}
}

void SectionTable::placeSection(string name, size_t place){
	if(sectionTable.count(name) == 0){
		SectionTableLine *sctnline = new SectionTableLine(name, place);
		sectionTable.insert(make_pair(name, sctnline));
	}
	else{
		SectionTableLine *sctnline = sectionTable[name];
		sctnline->base = place;
	}
}

void SectionTable::printAllSections(){
	for(auto it=sectionTable.begin(); it!=sectionTable.end(); it++){
		cout<<"#"<<it->first<<" "<<it->second->base<<" "<<it->second->content.size()/2<<endl;
	}
}