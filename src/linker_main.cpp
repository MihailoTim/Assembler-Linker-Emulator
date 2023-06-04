#include <iostream>
#include <string>
#include <algorithm>
#include "../inc/linker_exceptions.hpp"
#include "../inc/linker_parser.hpp"
#include "../inc/linker_symbolTable.hpp"
#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_printer.hpp"
using namespace std;

char *fileOut;

int main(int argc, char** argv) {
		int i=1;
		while(i<argc){
			string arg(argv[i]);
			if(arg == "-hex"){
				//set hex output
				i++;
			}
			else if(arg.find("-place") != string::npos){
				size_t place = arg.find("-place=") + 7;
				size_t at = arg.find("@");
				string section = arg.substr(place, at-place);
				string address = arg.substr(arg.find("@")+1);
				size_t location = stol(address, nullptr, 16);
				SectionTable::addNewSectionPlacement(section, location);
				cout<<section<<" "<<address<<endl;
				i++;
			}
			else if(arg == "-o"){
				fileOut = argv[i+1];
				i+=2;
			}
			else{
				char* fileIn = argv[i];
				Parser::parseFile(fileIn);
				i++;
			}
		}

	SectionTable::printAllSections();

	SectionTable::resolveSectionPlacements();

	SectionTable::upateSectionVirtualAddresses();

	SectionTable::printAllSections();

	for(auto it = SymbolTable::symbolTable.begin(); it != SymbolTable::symbolTable.end(); it++){
		string section = SymbolTable::symbolLookupTable[it->second->ndx];
		SectionTable::SectionTableLine *sctn = SectionTable::sectionTable[section];
		if(it->second->type != SymbolTable::SymbolType::SCTN)
			it->second->virtualAddress = sctn->base + it->second->offset;
	}

	for(auto reloLine : Parser::localReloTable){
		SymbolTable::SymbolTableLine *stline = SymbolTable::symbolTable[reloLine->symbol];
		SymbolTable::SymbolTableLine *referencePoint = SymbolTable::symbolTable[SymbolTable::symbolLookupTable[stline->ndx]];
		SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[reloLine->section];
		size_t replacement = (stline->type == SymbolTable::SymbolType::SCTN ? reloLine->sectionBase : 0) + stline->virtualAddress + reloLine->addend;
		cout<<reloLine->sectionBase<<" "<<stline->virtualAddress<<" "<<reloLine->addend<<endl;
		string bytes = Parser::get4Bytes(replacement);
		size_t location = reloLine->location; // OVDE DOHVATI BASE SEKCIJE
		cout<<"REPLACEMENT BYTES: "<<bytes<<endl;
		cout<<"REPLACEMENT LOCATION: "<<hex<<location<<endl;
		cout<<"REFERENCE POINT: "<<referencePoint->name<<" "<<referencePoint->offset<<endl;
		cout<<reloLine->location<<" "<<reloLine->symbol<<" "<<referencePoint->name<<" "<<stline->offset<<" "<<stline->name<<" "<<endl;
		sctnline->content.replace(reloLine->location*2, 8, bytes);
	}
	

	string res = "";
	for(auto it = SectionTable::sectionTable.begin(); it!=SectionTable::sectionTable.end();it++){
		cout<<"SECTIOM: "<<it->first<<endl;
		res+="\nSECTION: "+it->first+"\n";
		SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[it->first];
		cout<<sctnline->base<<" "<<sctnline->content.size()/2<<endl;
		for(int i=0;i<sctnline->content.size();i++){
			if(i%2 == 0 && i!=0){
				res+=" ";
			}
			if(i%16 == 0 && i!=0){
				res+="\n";
			}
			res+=sctnline->content[i];
		}
		cout<<res<<endl;
	}

	Printer::printAllSectionsToHex();

	SymbolTable::printSymbolTable();
	return 0;
}