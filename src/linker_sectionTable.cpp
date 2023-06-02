#include "../inc/linker_sectionTable.hpp"

map<string, SectionTable::SectionTableLine*> SectionTable::sectionTable;

void SectionTable::addNewSection(string name){
	if(sectionTable.count(name) == 0){
		SectionTableLine *sctnline = new SectionTableLine(name, 0);
		sctnline->content = "some string";
		sectionTable.insert(make_pair(name, sctnline));
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
		cout<<"#"<<it->first<<" "<<it->second->base<<" "<<it->second->content.size()<<endl;
	}
}