#include "../inc/linker_relocationTable.hpp"
#include "../inc/linker_parser.hpp"
#include "../inc/linker_relocationTable.hpp"
#include "../inc/linker_sectionTable.hpp"

vector<RelocationTable::RelocationTableLine*> RelocationTable::outputReloTable;

void RelocationTable::handleReloLine(size_t location, string type, string symbol, size_t addend){
	// cout<<location<<" "<<type<<" "<<symbol<<" "<<addend<<endl;
}

void RelocationTable::addReloLineToOutput(size_t location, string type, string symbol, size_t addend, string section, size_t base){
	RelocationTableLine* reloLine = new RelocationTableLine(location, type, symbol, addend, section, base);	
}

void RelocationTable::resolveRelocations(){
	for(auto reloLine : Parser::localReloTable){
		SymbolTable::SymbolTableLine *stline = SymbolTable::symbolTable[reloLine->symbol];
		SymbolTable::SymbolTableLine *referencePoint = SymbolTable::symbolTable[stline->name];
		SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[reloLine->section];
		size_t replacement = reloLine->sectionBase + stline->virtualAddress + reloLine->addend;
		string bytes = Parser::get4BytesLittleEndian(replacement);
		size_t location = reloLine->location; // OVDE DOHVATI BASE SEKCIJE

		RelocationTable::RelocationTableLine* outputReloLine = new RelocationTable::RelocationTableLine(location, reloLine->type, reloLine->symbol, reloLine->addend + reloLine->sectionBase, reloLine->section, reloLine->sectionBase );
		outputReloTable.push_back(outputReloLine);
		sctnline->content.replace(reloLine->location*2, 8, bytes);
	}
}

void RelocationTable::printRelocationsToOutput(ostream& out, string section){
	out<<"#.rela."<<section<<endl;
	for(auto outputReloLoine : RelocationTable::outputReloTable){
		SymbolTable::SymbolTableLine *stline = SymbolTable::symbolTable[outputReloLoine->symbol];
		if(outputReloLoine->section == section)
			out << outputReloLoine->location << " " << outputReloLoine->type << " " << stline->num << " " << outputReloLoine->addend<<endl;
	}
}