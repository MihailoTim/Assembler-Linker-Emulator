#include "../inc/assembler_symbolTable.hpp"
#include <iomanip>

void SymbolTable::handleGlobalDirective(string symbol){
	if(symbolTable.count(symbol)){
		SymbolTableLine &stline = symbolTable[symbol];
		if(stline.ndx == SymbolSection::EXTERN)
			throw new Exception("Conflicting directive for symbol:" + stline.name);
		if(stline.bind != SymbolBind::UNBOUND)
			stline.bind = SymbolBind::GLOB;
		stline.global = true;
	}
	else{
		SymbolTableLine *stline = handleNewSymbol(this->count++, 0, 0,SymbolType::NOTYPE, SymbolBind::UNBOUND, SymbolSection::GLOBAL, symbol);
		stline->global = true;
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::handleExternDirective(string symbol){
	if(symbolTable.count(symbol)){
		throw new Exception("Multiple definitions of symbol " + symbol);
	}
	else{
		SymbolTableLine *stline = handleNewSymbol(this->count++, 0, 0,SymbolType::NOTYPE, SymbolBind::GLOB, SymbolSection::EXTERN, symbol);
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::handleSectionDirective(string symbol, size_t locationCounter){
	if(sectionTable.count(symbol)>0){
		throw new Exception("Section "+ symbol + " has already been defined");
	}
	else{
		SectionTableLine &sctLine = sectionTable[currentSection];
		sctLine.length = locationCounter;
		currentSection = symbol;
		SectionTableLine newSctLine = *new SectionTableLine(0, 0, symbol, this->count);
		sectionTable.insert(make_pair(symbol, newSctLine));
		int num = this->count++;
		SymbolTableLine *stline = handleNewSymbol(num, 0, 0,SymbolType::SCTN, SymbolBind::LOC, num, symbol);
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::handleEquDirective(string symbol, size_t locationCounter){
	SymbolTableLine section = symbolTable[currentSection];
	if(symbolTable.count(symbol)>0){
		SymbolTableLine &stline = symbolTable[symbol];
		stline.ndx = section.num;
		if(stline.ndx == SymbolSection::GLOBAL){
			stline.bind = stline.global ? SymbolBind::GLOB : SymbolBind::LOC;
		}
		else if(stline.bind == SymbolBind::UNBOUND){
			stline.bind = stline.global ? SymbolBind::GLOB : SymbolBind::LOC;
		}
		else{
			throw new Exception("Symbol "+ symbol + " has already been defined");
		}
	}
	else{
		SymbolTableLine *stline = handleNewSymbol(this->count++, locationCounter, 0,SymbolType::NOTYPE, SymbolBind::LOC, section.num, symbol);
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::handleEndDirective(size_t locationCounter){
	SectionTableLine &sctLine = sectionTable[currentSection];
	sctLine.length = locationCounter;
	currentSection = "";
}

void SymbolTable::handleLabel(string symbol, size_t locationCounter){
	SymbolTableLine section = symbolTable[currentSection];
	if(symbolTable.count(symbol)>0){
		SymbolTableLine &stline = symbolTable[symbol];
		stline.ndx = section.num;
		if(stline.ndx == SymbolSection::GLOBAL){
			stline.value = locationCounter;
			stline.bind = stline.global ? SymbolBind::GLOB : SymbolBind::LOC;
		}
		else if(stline.bind == SymbolBind::UNBOUND){
			stline.value = locationCounter;
			stline.bind = stline.global ? SymbolBind::GLOB : SymbolBind::LOC;
		}
		else{
			throw new Exception("Symbol "+ symbol + " has already been defined");
		}
	}
	else{
		SymbolTableLine *stline = handleNewSymbol(this->count++, locationCounter, 0,SymbolType::NOTYPE, SymbolBind::LOC, section.num, symbol);
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::handleSymbolReference(string symbol, size_t locationCounter){
	SymbolTableLine section = symbolTable[currentSection];
	if(symbolTable.count(symbol) == 0){
		SymbolTableLine *stline = handleNewSymbol(this->count++, 0, 0,SymbolType::NOTYPE, SymbolBind::UNBOUND, section.num, symbol);
		symbolTable.insert(make_pair(symbol, *stline));
	}
}

void SymbolTable::printSymbolTable(){
	cout<<"#symbtab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		SymbolTableLine stline = symbolTable[it->second];
		cout<<stline.num<<" "<<stline.value<<" "<<stline.size<<" "<<stline.type<<" "<<stline.bind<<" "<<stline.ndx<<" "<<stline.name<<" ";
		for(auto ref : stline.references){
			cout<<"{"<<ref->locationCounter<<", "<<ref->refType<<"}, ";
		}
		cout<<endl;
	}
	cout<<"#lookuptab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		cout<<it->first<<" " <<it->second<<endl;
	}
}

void SymbolTable::printSymbolTable(ofstream &out){
	out<<"#symtab"<<endl;
	for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
		SymbolTableLine stline = symbolTable[it->second];
		out<<stline.num<<" "<<stline.value<<" "<<stline.size<<" "<<stline.type<<" "<<stline.bind<<" "<<stline.ndx<<" "<<stline.name<<endl;
	}
	// out<<"#lookuptab"<<endl;
	// for(auto it = symbolLookupTable.begin(); it!=symbolLookupTable.end();it++){
	// 	out<<it->first<<" " <<it->second<<endl;
	// }
}

void SymbolTable::printSectionTable(){
	cout<<"#sectiontab"<<endl;
	for(auto it = sectionTable.begin(); it!=sectionTable.end();it++){
		SectionTableLine stline = it->second;
		cout<<stline.base<<" "<<stline.length<<" "<<stline.name<<endl;
	}
}

void SymbolTable::printSectionTable(ofstream &out){
	out<<"#shdr"<<endl;
	for(auto it = sectionTable.begin(); it!=sectionTable.end();it++){
		SectionTableLine stline = it->second;
		out<<stline.base<<" "<<stline.length<<" "<<stline.name<<endl;
	}
}

SymbolTable::SymbolTableLine* SymbolTable::handleNewSymbol(size_t n, size_t v, size_t s, SymbolType st,  SymbolBind sb, int ind, string sname){
	symbolLookupTable.insert(make_pair(n, sname));
	return new SymbolTableLine(n, v, s, st, sb, ind, sname);
}

void SymbolTable::printSection(string section){
	SectionTableLine sctnline = sectionTable[section];
	string res = "";
	res += "#" + sctnline.name + "\n";
	for(int i=0;i<sctnline.content.size();i++){
		if(i%2 == 0 && i!=0){
			res+=" ";
		}
		if(i%8 == 0 && i!=0){
			res+=" ";
		}
		if(i%16 == 0 && i!=0){
			res+="\n";
		}
		res+=sctnline.content[i];
	}

	res += "\n#.rela." + sctnline.name + "\n";
	for(int i=0;i<sctnline.reloTable.size();i++){
		res += sctnline.reloTable[i] + "\n";
	}
}

void SymbolTable::printSection(string section, ofstream &out){
	SectionTableLine sctnline = sectionTable[section];
	string res = "";
	res += "#" + sctnline.name + "\n";
	out<<res;
	res="";
	for(int i=0;i<sctnline.content.size();i++){
		if(i%2 == 0 && i!=0){
			res+=" ";
		}
		if(i%16 == 0 && i!=0){
			res+="\n";
		}
		res+=sctnline.content[i];
	}
	if(res.size())
		out<<res<<endl;
	out<< "#.rela." + sctnline.name + "\n";
	for(int i=0;i<sctnline.reloTable.size();i++){
		out<< sctnline.reloTable[i] + "\n";
	}
}

void SymbolTable::printAllSections(){
	for(auto it = sectionTable.begin(); it!=sectionTable.end(); it++){
		printSection(it->first);
	}
}

void SymbolTable::printAllSections(ofstream& out){
	for(auto it = sectionTable.begin(); it!=sectionTable.end(); it++){
		printSection(it->first, out);
	}
}

void SymbolTable::printAllSectionsHexOnly(){
	ofstream out("debug.hex");
	for(auto it = sectionTable.begin(); it!=sectionTable.end(); it++){
		printSectionHexOnly(it->first, out);
	}
}

void SymbolTable::printSectionHexOnly(string section, ofstream &out){
	SectionTableLine sctnline = sectionTable[section];
	string res = "";
	// res += "#" + sctnline.name + "\n";
	out<<res;
	res="";
	for(int i=0;i<sctnline.content.size();i++){
		if(res.size() == 16){
			for(int j=3;j>=0;j--){
				out<<res.substr(j*2, 2)<<" ";
			}
			for(int j=7;j>=4;j--){
				out<<res.substr(j*2, 2)<< (j==4 ? "\n" : " ");
			}
			res="";
		}
		res+=sctnline.content[i];
	}
	if(res.size()){
		for(int j=min(3,int(res.size()/2));j>=0;j--){
			out<<res.substr(j*2, 2)<<" ";
		}
		for(int j=min(7,int(res.size()/2-4));j>=4;j--){
			out<<res.substr(4+j*2, 2)<< (j==4 ? "": " ");
		}
		out<<endl;
	}
	if(res.size() % 16 != 0){
		for(int i=7 - (res.size()%16)/2;i>=0;i--){
			res += "00";
		}
	}
	out<<endl;
}