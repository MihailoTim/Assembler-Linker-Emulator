#include "../h/secondPass.hpp"
#include "../h/assemblyInstruction.hpp"
#include <map>
#include "../h/secondPass.hpp"
#include "../h/literalPool.hpp"

string SecondPass::currentSection = "";
size_t SecondPass::locationCounter = 0;
SymbolTable* SecondPass::symbolTable = &SymbolTable::getInstance();
RelocationTable* SecondPass::reloTable = new RelocationTable("");
string SecondPass::sectionContent = "";
vector<string> SecondPass::reloTableContent;

void SecondPass::performBetweenPassCheck(){
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
	symbolTable->printSymbolTable();
	symbolTable->printSectionTable();
	performBetweenPassCheck();
	performLineByLine();
}

void SecondPass::performLineByLine(){
	for(auto line : file->readFile()){
		if(line->label.size()>0){
			SymbolTable::SymbolTableLine &stline = symbolTable->symbolTable[line->label];
			stline.value = locationCounter;
		}
		if(directiveHandlers.count(line->mnemonic))
			handleDirective(line);
		else
			if(instructionHandlers.count(line->mnemonic))
				handleInstruction(line);
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
	sctnline.reloTable = reloTable->getContent();
	currentSection = line->args[0]->stringVal;
	reloTable = new RelocationTable(currentSection);
	sectionContent = "";
	reloTableContent = *new vector<string>();
	locationCounter = 0;
}

void SecondPass::handleWordDirective(AssemblyLine* line){
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
		locationCounter += 4;
	}
}

void SecondPass::handleSkipDirective(AssemblyLine* line){
	cout<<"SKIP"<<endl;
	string content = "";
	for(int i=0;i<line->args[0]->intVal;i++){
		content += "00";
	}
	cout<<content<<endl;
	writeContentToSection(content);
	locationCounter += line->args[0]->intVal;
}

void SecondPass::handleAsciiDirective(AssemblyLine* line){
	cout<<line->args[0]->stringVal<<endl;
	locationCounter += line->args[0]->stringVal.size();
}

void SecondPass::handleEquDirective(AssemblyLine* line){
}

void SecondPass::handleEndDirective(AssemblyLine* line){
	SymbolTable::SectionTableLine &sctnline = symbolTable->sectionTable[currentSection];
	sctnline.content = sectionContent;
	sctnline.reloTable = reloTable->getContent();
	symbolTable->printAllSections();
	LiteralPool::printLiteralPool();
}

void SecondPass::handleHaltInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getHaltBytes(line);
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleIntInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getIntBytes(line);
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleIretInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getIretBytes(line);
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleCallInstruction(AssemblyLine* line) {
	int displ = handleCallArgument(line->args[0]);
	string content = AssemblyInstruction::getCallBytes(line, displ);
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleRetInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getRetBytes(line);
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleJmpInstruction(AssemblyLine* line) {
	cout<<"JMP"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[0], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleBeqInstruction(AssemblyLine* line) {
	cout<<"BEQ"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[2], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleBneInstruction(AssemblyLine* line) {
	cout<<"BNE"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[2], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleBgtInstruction(AssemblyLine* line) {
	cout<<"BGT"<<endl;
	bool useDispl = false;
	int displ = handleBranchArgument(line->args[2], useDispl);
	string content = AssemblyInstruction::getBranchBytes(line, displ, useDispl);
	cout<<content<<endl;
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handlePushInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getPushBytes(line);
    writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handlePopInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getPopBytes(line);
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleXchgInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getXchgBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleAddInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleSubInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleMulInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleDivInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleNotInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleAndInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleOrInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleXorInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleShlInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getShiftBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleShrInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getShiftBytes(line);
    locationCounter += 4;
	writeContentToSection(content);
}

void SecondPass::handleLdInstruction(AssemblyLine* line) {
	cout<<"LD"<<endl;
	int displ = handleLoadArgument(line->args[0]);
	string content = AssemblyInstruction::getLoadBytes(line, displ);
	cout<<content<<endl;
	writeContentToSection(content);
    locationCounter += content.size()/2;
}

void SecondPass::handleStInstruction(AssemblyLine* line) {
	cout<<"ST"<<endl;
	int displ = handleStoreArgument(line->args[1]);
	string content = AssemblyInstruction::getStoreBytes(line, displ);
	cout<<content<<endl;
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleCsrrdInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLoadBytes(line);
	writeContentToSection(content);
    locationCounter += 4;
}

void SecondPass::handleCsrwrInstruction(AssemblyLine* line) {
	string content = AssemblyInstruction::getLoadBytes(line);
	writeContentToSection(content);
    locationCounter += 4;
}

int SecondPass::handleBranchArgument(Argument *arg, bool &useDispl){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->argType == ArgumentType::SYM){
		SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
		if(sctline.symTabId == symline.ndx){
			if(canFitInDispl(symline.value, locationCounter + 4)){
				return symline.value - (locationCounter + 4);
			}
		}
		RelocationTable::RelocationTableLine* line = reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
		LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, 0, line);
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
		LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, 0, line);
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
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, 0, line);
		}
		if(arg->argType == ArgumentType::LITERAL){
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, arg->intVal, nullptr);
		}
	}
	if(arg->addrType == AddressType::REGINDOFF){
		if(arg->argType == ArgumentType::REGPLUSSYM){
			// SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			// string content = reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
			// writeReloContentToSection(content);
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
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, 0, line);
		}
		if(arg->argType == ArgumentType::LITERAL){
			LiteralPool::handleNewLiteralPoolEntry(locationCounter+2, arg->intVal, nullptr);
		}
	}
	if(arg->addrType == AddressType::REGINDOFF){
		if(arg->argType == ArgumentType::REGPLUSSYM){
			// SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			// string content = reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
			// writeReloContentToSection(content);
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