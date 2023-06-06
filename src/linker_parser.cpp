#include "../inc/linker_parser.hpp"
#include "../inc/linker_sectionTable.hpp"
#include "../inc/linker_relocationTable.hpp"
#include "../inc/linker_exceptions.hpp"
#include <iomanip>

using namespace std;

Parser::CurrentlyReading Parser::status = NOTHING;
string Parser::currentSection = "";
string Parser::sectionContent = "";
vector<SymbolTable::SymbolTableLine*> Parser::localSymbolTable;
map<string, string> Parser::localSectionContent;
vector<RelocationTable::RelocationTableLine*> Parser::localReloTable;
size_t Parser::currentBase = 0;

string Parser::getByte(uint8_t byte){
	string result="";
	std::stringstream sstream;
	sstream << setw(2) << setfill('0') << std::hex << int(byte);
	result = sstream.str();
	return result;
}

string Parser::get4Bytes(int bytes){
    uint8_t byte1 = (bytes >> 24) & 0xFF;
    uint8_t byte2 = (bytes >> 16) & 0xFF;
    uint8_t byte3 = (bytes >> 8) & 0xFF;
    uint8_t byte4 = bytes & 0xFF;

	return getByte(byte1) + getByte(byte2) + getByte(byte3) + getByte(byte4);
}

string Parser::get4BytesLittleEndian(int bytes){
    uint8_t byte1 = (bytes >> 24) & 0xFF;
    uint8_t byte2 = (bytes >> 16) & 0xFF;
    uint8_t byte3 = (bytes >> 8) & 0xFF;
    uint8_t byte4 = bytes & 0xFF;

	return getByte(byte4) + getByte(byte3) + getByte(byte2) + getByte(byte1);
}

string Parser::get4BytesLittleEndian(int byte1, int byte2, int byte3, int byte4){
	return getByte(byte4) + getByte(byte3) + getByte(byte2) + getByte(byte1);
}

void Parser::parseFile(char *fileIn){
	string line;
	ifstream in(fileIn);
	string filename = fileIn;

	if(!in.is_open()){
		throw new Exception("Couldn't open file "+filename);
	}

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
			currentBase = SectionTable::sectionTable[currentSection]->content.size()/2;
		}
		else{
			switch(status){
				case SECTIONHEADER : handleSectionLine(line); break;
				case SECTIONCONTENT : handleSectionContent(line); break;
				case SYMBOLTABLE : handleSymbolTableLine(line);break;
				case RELOCATIONS : handleRelocationLine(line);break;
			}
		}
	}

	for(auto stline : localSymbolTable){
		size_t n = SymbolTable::symbolTable.size();
		string section = stline->ndx >= 0 ? localSymbolTable[stline->ndx]->name : "";
		SymbolTable::insertNewSymbol(n, stline->offset, 0, stline->type, stline->bind, stline->ndx, stline->name, section);
	}
	localSymbolTable.clear();

	// SymbolTable::printSymbolTable();

	for(auto it = localSectionContent.begin(); it!=localSectionContent.end();it++){
		SectionTable::SectionTableLine *sctnline = SectionTable::sectionTable[it->first];
		sctnline->content = it->second;

		string res = "";
		// cout<<"SECTION: "<<it->first<<endl;
		for(int i=0;i<sctnline->content.size();i++){
			if(i%2 == 0 && i!=0){
				res+=" ";
			}
			if(i%8 == 0 && i!=0){
				res+=" ";
			}
			if(i%16 == 0 && i!=0){
				res+="\n";
			}
			res+=sctnline->content[i];
		}
		// cout<<res<<endl;
	}
	status = NOTHING;
	localSymbolTable.clear();

	sectionContent = "";
	currentSection = "";
}

void Parser::handleSectionLine(string line){
	size_t delim1 = line.find(" ");
	size_t base = stol(line.substr(0, delim1));
	line = line.substr(delim1+1);
	size_t delim2 = line.find(" ");
	size_t length = stol(line.substr(0, delim2));
	line = line.substr(delim2+1);
	string section = line;

	SectionTable::addNewSection(section, length);
}

void Parser::handleSectionContent(string line){
	string strippedLine = "";
	if(line.size()){
		for(char c : line){
			if(c != ' ' && c!='\n'){
				strippedLine+=c;
			}
		}
	}

	localSectionContent[currentSection] += strippedLine;
}

void Parser::handleSymbolTableLine(string line){
	size_t delim = line.find(" ");
	size_t num = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t value = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t size = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	SymbolTable::SymbolType type = (SymbolTable::SymbolType)stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	SymbolTable::SymbolBind bind = (SymbolTable::SymbolBind)stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	int ndx = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	string name = line;

	SymbolTable::SymbolTableLine *stline = new SymbolTable::SymbolTableLine(num, value, 0, type, bind, ndx, name);

	for(auto line : localSymbolTable){
		if(line->name == stline->name)
			return;
	}
	localSymbolTable.push_back(stline);
}

void Parser::handleRelocationLine(string line){
	size_t delim = line.find(" ");
	size_t location = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	string type = line.substr(0, delim);
	line = line.substr(delim+1);
	delim = line.find(" ");
	size_t symbolIndex = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	delim = line.find(" ");
	int addend = stol(line.substr(0, delim));
	line = line.substr(delim+1);
	string symbol = localSymbolTable[symbolIndex]->name;

	cout<<symbol<<" "<<location<<" "<<type<<" "<<currentSection<<endl;

	localReloTable.push_back(new RelocationTable::RelocationTableLine(currentBase + location, type, symbol, addend, currentSection, currentBase));
}