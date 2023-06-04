#include "../inc/assembler_secondPass.hpp"
#include "../inc/assembler_assemblyInstruction.hpp"
#include <map>
#include "../inc/assembler_secondPass.hpp"
#include "../inc/assembler_literalPool.hpp"

string SecondPass::currentSection = "";
size_t SecondPass::locationCounter = 0;
size_t SecondPass::previousLocationCounter = 0;
SymbolTable* SecondPass::symbolTable = &SymbolTable::getInstance();
RelocationTable* SecondPass::reloTable = new RelocationTable("");
string SecondPass::sectionContent = "";
vector<string> SecondPass::reloTableContent;

void SecondPass::performBetweenPassCheck(){
	symbolTable->printSymbolTable();
	map<string, SymbolTable::SymbolTableLine> symTab = symbolTable->symbolTable;
	for(auto stline : symTab){
		if(stline.second.ndx == SymbolTable::SymbolSection::GLOBAL || stline.second.bind == SymbolTable::SymbolBind::UNBOUND){
			throw new Exception("Symbol " + stline.first + " is being used but is not declared");
		}
	}
}

void SecondPass::start(){
	status = RUNNING;
	setSymbolTable(&SymbolTable::getInstance());
	performBetweenPassCheck();
	performLineByLine();
	symbolTable->printAllSections();
	symbolTable->printSymbolTable();
	symbolTable->printSectionTable();
	dumpContentToFile();
}

void SecondPass::performLineByLine(){
	for(auto line : file->readFile()){
		locationCounter = sectionContent.size() / 2;
		if(line->label.size()){
			cout<<locationCounter<<" "<<line->label<<endl;
			SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[line->label];
			symline.value = locationCounter;
		}
		if(directiveHandlers.count(line->mnemonic))
			handleDirective(line);
		else
			if(instructionHandlers.count(line->mnemonic))
				handleInstruction(line);	
		LiteralPool::conditionalDumpPool(sectionContent, locationCounter, previousLocationCounter);
		locationCounter = sectionContent.size()/2;
		if(line->mnemonic.size()){
			previousLocationCounter = locationCounter;
		}
	}
}

void SecondPass::handleDirective(AssemblyLine *line){
	directiveHandlers[line->mnemonic](line);
}

void SecondPass::handleInstruction(AssemblyLine* line){
	instructionHandlers[line->mnemonic](line);
}

void SecondPass::handleGlobalDirective(AssemblyLine* line){
	return;
}

void SecondPass::handleExternDirective(AssemblyLine* line){
	return;
}

void SecondPass::handleSectionDirective(AssemblyLine* line){
	SymbolTable::SectionTableLine &sctnline = symbolTable->sectionTable[currentSection];
	sctnline.content = sectionContent;
	LiteralPool::printLiteralPool();
	LiteralPool::dumpPool(sctnline.content, locationCounter);
	sctnline.length = locationCounter - sctnline.base;
	cout<<currentSection<<endl;
	fixSymbolReferences();
	fixReloTable();
	LiteralPool::changeSection();
	sctnline.reloTable = reloTable->getContent();
	currentSection = line->args[0]->stringVal;
	reloTable = new RelocationTable(currentSection);
	sectionContent = "";
	reloTableContent = *new vector<string>();
	locationCounter = 0;
}

void SecondPass::handleWordDirective(AssemblyLine* line){
	cout<<locationCounter<<": WORD"<<endl;
	for(auto arg : line->args){
		if(arg->argType == ArgumentType::LITERAL){
			string content = AssemblyInstruction::get4Bytes(arg->intVal);
			writeContentToSection(content);
		}
		else{
			if(arg->argType == ArgumentType::SYM){
				reloTable->handleNewReloLine(locationCounter,RelocationTable::RelocationType::R_32, arg->stringVal);
				string content = "00 00 00 00";
				writeContentToSection(content);
			}
		}
		//locationCounter += 4;
	}
}

void SecondPass::handleSkipDirective(AssemblyLine* line){
	cout<<locationCounter<<": SKIP"<<endl;
	string content = "";
	for(int i=0;i<line->args[0]->intVal;i++){
		content += "00";
	}
	cout<<content<<endl;
	writeContentToSection(content);
	locationCounter += line->args[0]->intVal;
}

void SecondPass::handleAsciiDirective(AssemblyLine* line){
	string str = "";
	for(char c : line->args[0]->stringVal){
		str += AssemblyInstruction::getByte(c);
	}
	for(int i=0;i<(4-line->args[0]->stringVal.size()%4)%4;i++)
		str += AssemblyInstruction::getByte(0);
	cout<<"ASCII: "<<str<<endl;
	locationCounter += str.size();			
	string res = "";
	for(int i=0;i<str.size();i++){
		if(res.size() == 8){
			for(int j=3;j>=0;j--){
				sectionContent += res.substr(j*2, 2);
			}
			res="";
		}
		res+=str[i];
	}
	if(res.size()){
		for(int j=min(3,int(res.size()/2));j>=0;j--){
			sectionContent += res.substr(j*2, 2);
		}
	}
}

void SecondPass::handleEquDirective(AssemblyLine* line){
	cout<<"EQU\n"<<endl;
}

void SecondPass::handleEndDirective(AssemblyLine* line){
	SymbolTable::SectionTableLine &sctnline = symbolTable->sectionTable[currentSection];
	sctnline.content = sectionContent;
	LiteralPool::dumpPool(sctnline.content, locationCounter);
	fixSymbolReferences();
	fixReloTable();
	sctnline.length = locationCounter - sctnline.base;
	sctnline.reloTable = reloTable->getContent();
}

void SecondPass::handleHaltInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": HALT"<<endl;
	string content = AssemblyInstruction::getHaltBytes(line);
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleIntInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getIntBytes(line);
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleIretInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getIretBytes(line);
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleCallInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": CALL"<<endl;
	int displ = handleCallArgument(line->args[0]);
	string content = AssemblyInstruction::getCallBytes(line, displ);
	cout<<content<<endl;
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleRetInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getRetBytes(line);
	cout<<locationCounter<<": RET"<<endl;
	cout<<content<<endl;
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleJmpInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": JMP"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[0], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleBeqInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": BEQ"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[2], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleBneInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": BNE"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[2], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleBgtInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": BGT"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[2], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handlePushInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getPushBytes(line);
	cout<<locationCounter<<": PUSH"<<endl;
	cout<<content<<endl;
    writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handlePopInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getPopBytes(line);
	cout<<locationCounter<<": POP"<<endl;
	cout<<content<<endl;
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleXchgInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getXchgBytes(line);
	cout<<locationCounter<<": XCHG"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleAddInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<locationCounter<<": ADD"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleSubInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<locationCounter<<": SUB"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleMulInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<locationCounter<<": MUL"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleDivInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<locationCounter<<": DIV"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleNotInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<locationCounter<<": NOT"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleAndInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<locationCounter<<": AND"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleOrInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<locationCounter<<": OR"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleXorInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<locationCounter<<": XOR"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleShlInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getShiftBytes(line);
	cout<<locationCounter<<": SHL"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleShrInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getShiftBytes(line);
	cout<<locationCounter<<": SHR"<<endl;
	cout<<content<<endl;
    //locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleLdInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": LD"<<endl;
	int displ = handleLoadArgument(line->args[0]);
	string content = AssemblyInstruction::getLoadBytes(line, displ);
	cout<<content<<endl;
	writeContentToSection(content);
    locationCounter += content.size()/2;
}

void SecondPass::handleStInstruction(AssemblyLine* line) {
	cout<<locationCounter<<": ST"<<endl;
	int displ = handleStoreArgument(line->args[1]);
	string content = AssemblyInstruction::getStoreBytes(line, displ);
	cout<<content<<endl;
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleCsrrdInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLoadBytes(line);
	cout<<locationCounter<<": CSRRD"<<endl;
	cout<<content<<endl;
	writeContentToSection(content);
    //locationCounter += 4;
}

void SecondPass::handleCsrwrInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLoadBytes(line);
	cout<<locationCounter<<": CSRWR"<<endl;
	cout<<content<<endl;
	writeContentToSection(content);
    //locationCounter += 4;
}

int SecondPass::handleBranchArgument(Argument *arg, bool &useDispl){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->argType == ArgumentType::SYM){
		SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
		// if(sctline.symTabId == symline.ndx){
		// 	if(canFitInDispl(symline.value, locationCounter + 4)){
		// 		useDispl = true;
		// 		if((int)symline.value - ((int)locationCounter + 4) > 0){
		// 			LiteralPool::handleForwardBranch(locationCounter+2, symline.value - (locationCounter + 4));
		// 		}
		// 		symline.addNewReference(locationCounter, SymbolTable::ReferenceLocation::DIRECT, locationCounter+4);
		// 		return symline.value - (locationCounter + 4);
		// 	}
		// }
		RelocationTable::RelocationTableLine* line = reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
		symline.addNewReference(locationCounter, SymbolTable::ReferenceLocation::INDIRECT, locationCounter + 4);
		LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, symline.value, line);
	}
	if(arg->argType == ArgumentType::LITERAL){
		LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, arg->intVal, nullptr);
	}
	return 0;
}

int SecondPass::handleCallArgument(Argument *arg){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->argType == ArgumentType::SYM){
		SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
		RelocationTable::RelocationTableLine* line = reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
		LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, symline.value, line);
		symline.addNewReference(locationCounter, SymbolTable::ReferenceLocation::INDIRECT, locationCounter+4);
	}
	if(arg->argType == ArgumentType::LITERAL){
		LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, arg->intVal, nullptr);
	}
	return 0;
}

int SecondPass::handleLoadArgument(Argument *arg){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->addrType == IMMED || arg->addrType == MEMDIR){
		if(arg->argType == ArgumentType::SYM){
			SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			RelocationTable::RelocationTableLine* line = reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, symline.value, line);
		}
		if(arg->argType == ArgumentType::LITERAL){
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, arg->intVal, nullptr);
		}
	}
	if(arg->addrType == AddressType::REGINDOFF){
		if(arg->argType == ArgumentType::REGPLUSSYM){
			throw new Exception("Symbol whose value cannot be determined is being used in regind addressing");
			// SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			// symline.addNewReference(locationCounter, SymbolTable::ReferenceLocation::DIRECT, 0);
		}
		if(arg->argType == ArgumentType::REGPLUSLIT){
			if(canFitInDispl(stoi(arg->stringVal), 0))
				return stoi(arg->stringVal);
			else
				throw new Exception("Displacement can't fit in instruction");
		}
	}
	return 0;
}

int SecondPass::handleStoreArgument(Argument *arg){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->addrType == IMMED || arg->addrType == MEMDIR){
		if(arg->argType == ArgumentType::SYM){
			SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			RelocationTable::RelocationTableLine* line = reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, symline.value, line);
		}
		if(arg->argType == ArgumentType::LITERAL){
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, arg->intVal, nullptr);
		}
	}
	if(arg->addrType == AddressType::REGINDOFF){
		if(arg->argType == ArgumentType::REGPLUSSYM){
			throw new Exception("Symbol whose value cannot be determined is being used in regind addressing");
			// SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			// symline.addNewReference(locationCounter, SymbolTable::ReferenceLocation::DIRECT, 0);
		}
		if(arg->argType == ArgumentType::REGPLUSLIT){
			if(canFitInDispl(stoi(arg->stringVal), 0))
				return stoi(arg->stringVal);
			else
				throw new Exception("Displacement can't fit in instruction");
		}
	}
	return 0;
}

void SecondPass::writeContentToSection(string content){
	sectionContent += content;
}

void SecondPass::writeReloContentToSection(string content)
{
	// reloTableContent.push_back(content);
}

void SecondPass::dumpContentToFile(){
	symbolTable->printAllSectionsHexOnly();
	symbolTable->printSectionTable(objectFile);
	symbolTable->printSymbolTable(objectFile);
	symbolTable->printAllSections(objectFile);
}

void SecondPass::fixSymbolReferences(){
	SymbolTable::SectionTableLine &sctnline = symbolTable->sectionTable[currentSection];
	for(auto it = symbolTable->symbolTable.begin(); it!=symbolTable->symbolTable.end();it++){
		SymbolTable::SymbolTableLine &stline = symbolTable->symbolTable[it->first];
		for(auto ref : stline.references){
			int displ  = stline.value - ref->refPoint;
			cout<<"DISPLACEMENT: "<<displ<<endl;
			cout<<stline.name<<stline.value<<ref->refPoint<<endl;
			cout<<sctnline.content.size()<<endl;
			cout<<currentSection<<endl;
			if(ref->refType == SymbolTable::ReferenceLocation::DIRECT){
				string byte3 = AssemblyInstruction::getByte((displ >> 8) & 0xF);
				string byte4 = AssemblyInstruction::getByte(displ & 0xFF);
				string replacement = byte3 + byte4;
				sctnline.content.replace((ref->locationCounter*2 + 5), 3, replacement.substr(1));
			}
		}
		stline.references.clear();
	}
}

void SecondPass::fixReloTable(){
	RelocationTable *newReloTable = new RelocationTable(currentSection);

	for(auto it = reloTable->reloTable.begin();it!=reloTable->reloTable.end();it++){
		newReloTable->handleNewReloLine(it->second->offset, RelocationTable::RelocationType::R_32, it->second->relocatedSymbol);
	}

	reloTable = newReloTable;
}