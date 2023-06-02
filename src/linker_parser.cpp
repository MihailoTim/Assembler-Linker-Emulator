#include "../inc/linker_parser.hpp"
#include "../inc/linker_sectionTable.hpp"

using namespace std;

Parser::CurrentlyReading Parser::status = NOTHING;
string Parser::currentSection = "";

void Parser::parseFile(char *fileIn){
	string line;
	ifstream in(fileIn);

	while(getline(in, line)){
		if(line.find("#shdr") != string::npos){
			status = SECTIONHEADER;
		}
		else if(line.find("#.rela.") != string::npos){
			status = RELOCATIONS;
			currentSection = line.substr(7);
		}
		else if(line.find("#symtab") != string::npos){
			status = SYMBOLTABLE;
		}
		else if(line.find("#") != string::npos){
			status = SECTIONCONTENT;
			currentSection = line.substr(1);
		}
		else{
			switch(status){
				case SECTIONHEADER : handleSectionLine(line); break;
				case SECTIONCONTENT : handleSectionContent(line); break;
				case SYMBOLTABLE : handleSymbolTableLine(line);break;
			}
		}
	}
	status = NOTHING;
}


void Parser::getStatus(string line){
	if(line.find("#shdr") != string::npos){
		status = SECTIONHEADER;
	}
	else if(line.find("#.rela.") != string::npos){
		status = RELOCATIONS;
		currentSection = line.substr(7);
	}
	else if(line.find("#symtab") != string::npos){
		status = SYMBOLTABLE;
	}
	else if(line.find("#") != string::npos){
		status = SECTIONCONTENT;
		currentSection = line.substr(1);
	}
}

void Parser::handleSectionLine(string line){
	size_t delim1 = line.find(" ");
	size_t base = stol(line.substr(0, delim1));
	line = line.substr(delim1+1);
	size_t delim2 = line.find(" ");
	size_t length = stol(line.substr(0, delim2));
	line = line.substr(delim2+1);
	string section = line;
	currentSection = section;
	SectionTable::addNewSection(section);
}

void Parser::handleSectionContent(string line){
	string strippedLine = "";
	if(line.size()){
		line = line.substr(10);
		for(char c : line){
			if(c != ' '){
				strippedLine+=c;
			}
		}
	}
	SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[currentSection];
	cout<<strippedLine<<endl;
}

void Parser::handleSymbolTableLine(string line){
	size_t delim = line.find(" ");
	size_t num = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t offset = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t size = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t type = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t bind = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t ndx = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	string name = line;

	cout<<ndx<<" "<<name<<endl;
}