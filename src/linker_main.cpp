#include <iostream>
#include <string>
#include <algorithm>
#include "../inc/linker_exceptions.hpp"
#include "../inc/linker_parser.hpp"
#include "../inc/linker_symbolTable.hpp"
#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_printer.hpp"
using namespace std;

string fileOut("default.hex");

enum LinkerMode{UNDEFINED, RELOCATABLE, HEX};

int main(int argc, char** argv) {
	int i=1;
	LinkerMode mode = UNDEFINED;
	while(i<argc){
		string arg(argv[i]);
		if(arg == "-hex"){
			if(mode == LinkerMode::RELOCATABLE)
				throw new Exception("Both -hex and -relocatable have been used");
			mode = LinkerMode::HEX;
			i++;
		}
		else if(arg == "-relocatable"){
			if(mode == LinkerMode::HEX)
				throw new Exception("Both -hex and -relocatable have been used");
			mode = LinkerMode::RELOCATABLE;
			i++;
		}
		else if(arg.find("-place") != string::npos){
			size_t place = arg.find("-place=") + 7;
			size_t at = arg.find("@");
			string section = arg.substr(place, at-place);
			string address = arg.substr(arg.find("@")+1);
			size_t location = stol(address, nullptr, 16);
			SectionTable::addNewSectionPlacement(section, location);
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

	if(mode == LinkerMode::HEX){

		SectionTable::resolveSectionPlacements();

		SectionTable::upateSectionVirtualAddresses();

		SymbolTable::updateSymbolVirtualAddresses();

		SymbolTable::updateEquSymbolVirtualAddresses();

		RelocationTable::resolveRelocations();

		bool check = SymbolTable::checkHexCompatible();
		if(!check){
			throw new Exception("Can't make hex as there are unresolved symbols");
		}
		Printer::printAllSectionsToHex(fileOut.c_str());
	}
	else if(mode == LinkerMode::RELOCATABLE){

		//SectionTable::upateSectionVirtualAddresses();

		SymbolTable::updateSymbolVirtualAddresses();

		SymbolTable::updateEquSymbolVirtualAddresses();

		RelocationTable::resolveRelocations();


		ofstream out(fileOut);
		SectionTable::printSectionHeadersToOutput(out);

		SymbolTable::printSymbolTableToOutput(out);

		SectionTable::printSectionContentToOutput(out);

		// SymbolTable::printSymbolTable();
	}
	return 0;
}