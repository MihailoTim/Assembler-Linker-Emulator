#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_symbolTable.hpp"
#include <algorithm>

map<string, SectionTable::SectionTableLine*> SectionTable::sectionTable;
map<string, size_t> SectionTable::sectionPlacements;
size_t SectionTable::totalSize = 0;

void SectionTable::addNewSection(string name, size_t length){
	if(sectionTable.count(name) == 0){
		SectionTableLine *sctnline = new SectionTableLine(name, totalSize);
		sectionTable.insert(make_pair(name, sctnline));
		int ndx = SymbolTable::symbolTable.size();
		SymbolTable::SymbolTableLine *stline = new SymbolTable::SymbolTableLine(ndx, totalSize, 0, SymbolTable::SymbolType::SCTN, SymbolTable::SymbolBind::LOC, ndx, name);
		SymbolTable::symbolTable.insert(make_pair(name, stline));
		SymbolTable::symbolLookupTable.insert(make_pair(ndx, name));
		totalSize += length;
	}
	else{
		totalSize += length;
	}
}

void SectionTable::printAllSections(){
	for(auto it=sectionTable.begin(); it!=sectionTable.end(); it++){
		cout<<"#"<<it->first<<" "<<it->second->base<<" "<<it->second->content.size()/2<<endl;
	}
}

void SectionTable::addNewSectionPlacement(string section, size_t location){
	cout<<"NEW SECTION PLACEMENT: "<<location<<endl;
	sectionPlacements.insert(make_pair(section, location));
}

void SectionTable::resolveSectionPlacements(){
	for(auto it = sectionPlacements.begin(); it!= sectionPlacements.end(); it++){
		sectionTable[it->first]->base = it->second;
		SymbolTable::SymbolTableLine *stline = SymbolTable::symbolTable[it->first];
		stline->virtualAddress = sectionTable[it->first]->base;
	}
}

void SectionTable::upateSectionVirtualAddresses(){
	size_t totalSize = 0;

	vector<pair<size_t, string>> sortedSections;
	vector<pair<size_t, string>> sortedPlacedSections;

	for(auto it=SectionTable::sectionTable.begin(); it!=SectionTable::sectionTable.end(); it++){
		sortedSections.push_back(make_pair(it->second->base, it->first));
		if(sectionPlacements.count(it->first)){
			sortedPlacedSections.push_back(make_pair(it->second->base, it->first));
		}
	}


	sort(sortedSections.begin(), sortedSections.end());
	sort(sortedPlacedSections.begin(), sortedPlacedSections.end());

	for(auto it = sectionPlacements.begin(); it!= sectionPlacements.end(); it++){
		cout<<"SECTION: "<<it->first<<" PLACE AT: "<<it->second<<endl;
	}

	auto it = sortedPlacedSections.begin();
	for(auto section : sortedSections){

		SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[section.second];
		if(sectionPlacements.count(sctnline->name) == 0 && sctnline->content.size()){
			cout<<"SECTION NAME: "<<section.second<<endl;
			cout<<totalSize + sctnline->content.size()/2<<" "<<it->first<<endl;
			while(it != sortedPlacedSections.end() && totalSize + sctnline->content.size()/2 > it->first){
				SectionTableLine* placedSection = sectionTable[it->second];
				cout<<"CANT FIT "<<sctnline->name<<" AT: "<<totalSize<<endl;
				totalSize = it->first + placedSection->content.size()/2;
				it++;
			}
			cout<<"TOTAL SIZE: "<<totalSize<<endl;
			sctnline->base = totalSize;
			totalSize += sctnline->content.size()/2;
			SymbolTable::SymbolTableLine *stline = SymbolTable::symbolTable[section.second];
			stline->virtualAddress = sctnline->base;
		}
	}

	// for(auto it: sortedSections){
	// 	SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[it.second];
	// 	if(sectionPlacements.count(sctnline->name) == 0)
	// 		sctnline->base = totalSize;
	// 	totalSize += sctnline->content.size()/2;
	// 	SymbolTable::SymbolTableLine *stline = SymbolTable::symbolTable[it.second];
	// 	stline->virtualAddress = sctnline->base;
	// }
}