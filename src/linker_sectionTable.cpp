#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_symbolTable.hpp"
#include "../inc/linker_relocationTable.hpp"
#include <algorithm>
#include <fstream>

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
		cout<<"#"<<it->first<<" "<<hex<<it->second->base<<" "<<it->second->content.size()/2<<endl;
	}
}

void SectionTable::addNewSectionPlacement(string section, size_t location){
	sectionPlacements.insert(make_pair(section, location));
}

void SectionTable::resolveSectionPlacements(){
	SectionTable::printAllSections();
	for(auto it = sectionPlacements.begin(); it!= sectionPlacements.end(); it++){
		sectionTable[it->first]->base = it->second;
		cout<<it->first<<" "<<it->second<<" "<<endl;
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

	auto it = sortedPlacedSections.begin();
	for(auto section : sortedSections){

		SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[section.second];
		if(sectionPlacements.count(sctnline->name) == 0 && sctnline->content.size()){
			while(it != sortedPlacedSections.end() && totalSize + sctnline->content.size()/2 > it->first){
				SectionTableLine* placedSection = sectionTable[it->second];
				totalSize = it->first + placedSection->content.size()/2;
				it++;
			}
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

void SectionTable::printSectionHeadersToOutput(ofstream &out){
	out<<"#shdr\n";
	for(auto it = SectionTable::sectionTable.begin(); it!=SectionTable::sectionTable.end(); it++){
		out<<it->second->base<<" "<<it->second->content.size()/2<<" "<<it->first<<endl;
	}
}

void SectionTable::printSectionContentToOutput(ofstream &out){
	for(auto it = SectionTable::sectionTable.begin(); it!=SectionTable::sectionTable.end(); it++){
		SectionTableLine *sctnline = it->second;
		string res = "";
		res += "#" + sctnline->name + "\n";
		out<<res;
		res="";
		for(int i=0;i<sctnline->content.size();i++){
			if(i%2 == 0 && i!=0){
				res+=" ";
			}
			if(i%16 == 0 && i!=0){
				res+="\n";
			}
			res+=sctnline->content[i];
		}
		if(res.size())
			out<<res<<endl;
	
		RelocationTable::printRelocationsToOutput(out, it->second->name);
		
	}	
}