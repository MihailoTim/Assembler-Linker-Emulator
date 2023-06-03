#include <iostream>
#include <string>
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
			cout<<arg<<endl;
			if(arg == "-hex"){
				//set hex output
				i++;
			}
			else if(arg.find("-place") != string::npos){
				//place section
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
	for(auto reloLine : Parser::localReloTable){
		SymbolTable::SymbolTableLine *stline = SymbolTable::symbolTable[reloLine->symbol];
		SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[reloLine->section];
		size_t replacement = stline->value + reloLine->addend;
		string bytes = Parser::get4Bytes(replacement);
		size_t location = reloLine->location; // OVDE DOHVATI BASE SEKCIJE
		cout<<"REPLACEMENT BYTES: "<<bytes<<endl;
		cout<<"REPLACEMENT LOCATION: "<<location<<endl;
		sctnline->content.replace(reloLine->location*2, 8, bytes);
		cout<<reloLine->location<<" "<<reloLine->symbol<<" "<<reloLine->section<<" "<<stline->value<<endl;
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