#include "../h/secondPass.hpp"
#include "../h/assemblyInstruction.hpp"
#include <map>

string SecondPass::currentSection = "";
size_t SecondPass::locationCounter = 0;
SymbolTable* SecondPass::symbolTable = &SymbolTable::getInstance();
RelocationTable* SecondPass::reloTable = new RelocationTable("");

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
	cout<<locationCounter<<endl;
	currentSection = line->args[0]->stringVal;
	reloTable = new RelocationTable(currentSection);
	locationCounter = 0;
	cout<<currentSection<<endl;
}

void SecondPass::handleWordDirective(AssemblyLine* line){
	for(auto arg : line->args){
		if(arg->argType == ArgumentType::LITERAL){
			string content = AssemblyInstruction::get4Bytes(arg->intVal);
			cout<<content<<endl;
		}
		else{
			if(arg->argType == ArgumentType::SYM){
				reloTable->handleNewReloLine(locationCounter,RelocationTable::RelocationType::R_32, arg->stringVal);
				string content = "00 00 00 00";
				cout<<content<<endl;
			}
		}
		locationCounter += 4;
	}
	cout<<"WORD\n";
}

void SecondPass::handleSkipDirective(AssemblyLine* line){
	cout<<"SKIP\n";
	string content = "";
	for(int i=0;i<line->args[0]->intVal;i++){
		if(i%4 == 0 && i!=0)
			content += " ";
		if(i%8 == 0 && i!=0)
			content += "\n";
		content += "00 ";
	}
	cout<<content<<endl;
	locationCounter += line->args[0]->intVal;
}

void SecondPass::handleAsciiDirective(AssemblyLine* line){
	cout<<line->args[0]->stringVal<<endl;
	locationCounter += line->args[0]->stringVal.size();
	cout<<"ASCII\n";
}

void SecondPass::handleEquDirective(AssemblyLine* line){
	cout<<"EQU\n";
}

void SecondPass::handleEndDirective(AssemblyLine* line){
	cout<<locationCounter<<endl;
	cout<<"END\n";
}

void SecondPass::handleHaltInstruction(AssemblyLine* line) {
    std::cout << "HALT\n";
	string content = AssemblyInstruction::getHaltBytes();
    locationCounter += 4;
}

void SecondPass::handleIntInstruction(AssemblyLine* line) {
    std::cout << "INT\n";
	string content = AssemblyInstruction::getIntBytes();
    locationCounter += 4;
}

void SecondPass::handleIretInstruction(AssemblyLine* line) {
    std::cout << "IRET\n";
    locationCounter += 4;
}

void SecondPass::handleCallInstruction(AssemblyLine* line) {
    std::cout << "CALL\n";
	int displ = handleCallArgument(line->args[0]);
	string content = AssemblyInstruction::getCallBytes(line, displ);
	cout<<content<<endl;
    locationCounter += 4;
}

void SecondPass::handleRetInstruction(AssemblyLine* line) {
    std::cout << "RET\n";
    locationCounter += 4;
}

void SecondPass::handleJmpInstruction(AssemblyLine* line) {
    cout << "JMP\n";
	int displ = handleBranchArgument(line->args[0]);
	string content = AssemblyInstruction::getBranchBytes(line, displ);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleBeqInstruction(AssemblyLine* line) {
    std::cout << "BEQ\n";
	int displ = handleBranchArgument(line->args[2]);
	string content = AssemblyInstruction::getBranchBytes(line, displ);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleBneInstruction(AssemblyLine* line) {
    std::cout << "BNE\n";
	int displ = handleBranchArgument(line->args[2]);
	string content = AssemblyInstruction::getBranchBytes(line, displ);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleBgtInstruction(AssemblyLine* line) {
    std::cout << "BGT\n";
	int displ = handleBranchArgument(line->args[2]);
	string content = AssemblyInstruction::getBranchBytes(line, displ);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handlePushInstruction(AssemblyLine* line) {
    std::cout << "PUSH\n";
    locationCounter += 4;
}

void SecondPass::handlePopInstruction(AssemblyLine* line) {
    std::cout << "POP\n";
    locationCounter += 4;
}

void SecondPass::handleXchgInstruction(AssemblyLine* line) {
    std::cout << "XCHG\n";
	string content = AssemblyInstruction::getXchgBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleAddInstruction(AssemblyLine* line) {
    std::cout << "ADD\n";
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleSubInstruction(AssemblyLine* line) {
    std::cout << "SUB\n";
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleMulInstruction(AssemblyLine* line) {
    std::cout << "MUL\n";
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleDivInstruction(AssemblyLine* line) {
    std::cout << "DIV\n";
	string content = AssemblyInstruction::getArithmBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleNotInstruction(AssemblyLine* line) {
    std::cout << "NOT\n";
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleAndInstruction(AssemblyLine* line) {
    std::cout << "AND\n";
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleOrInstruction(AssemblyLine* line) {
    std::cout << "OR\n";
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleXorInstruction(AssemblyLine* line) {
    std::cout << "XOR\n";
	string content = AssemblyInstruction::getLogicBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleShlInstruction(AssemblyLine* line) {
    std::cout << "SHL\n";
	string content = AssemblyInstruction::getShiftBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleShrInstruction(AssemblyLine* line) {
    std::cout << "SHR\n";
	string content = AssemblyInstruction::getShiftBytes(line);
    locationCounter += 4;
	cout<<content<<endl;
}

void SecondPass::handleLdInstruction(AssemblyLine* line) {
    std::cout << "LD\n";
	int displ = handleLoadArgument(line->args[0]);
	string content = AssemblyInstruction::getLoadBytes(line, displ);
	cout<<content<<endl;
    locationCounter += 4;
}

void SecondPass::handleStInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "ST\n";
}

void SecondPass::handleCsrrdInstruction(AssemblyLine* line) {
    std::cout << "CSRRD\n";
	string content = AssemblyInstruction::getLoadBytes(line);
	cout<<content<<endl;
    locationCounter += 4;
}

void SecondPass::handleCsrwrInstruction(AssemblyLine* line) {
    std::cout << "CSRWR\n";
	string content = AssemblyInstruction::getLoadBytes(line);
	cout<<content<<endl;
    locationCounter += 4;
}

int SecondPass::handleBranchArgument(Argument *arg){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->argType == ArgumentType::SYM){
		SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
		if(sctline.symTabId == symline.ndx){
			if(canFitInDispl(symline.value, locationCounter + 4)){
				return symline.value - (locationCounter + 4);
			}
			else{
				cout<<"LITERAL POOL IS NEEDED\n";
			}
		}
		else{
			reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_PC32, arg->stringVal);
		}
	}
	if(arg->argType == ArgumentType::LITERAL){
		if(canFitInDispl(arg->intVal, locationCounter + 4))
			return arg->intVal - (locationCounter + 4);
		else
			cout<<"LITERAL POOL IS NEEDED\n";
	}
	return 0;
}

int SecondPass::handleCallArgument(Argument *arg){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->argType == ArgumentType::SYM){
		SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
		reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_PC32, arg->stringVal);
	}
	if(arg->argType == ArgumentType::LITERAL){
		if(canFitInDispl(arg->intVal, 0))
			return arg->intVal;
		else
			cout<<"LITERAL POOL IS NEEDED\n";
	}
	return 0;
}

int SecondPass::handleLoadArgument(Argument *arg){
	SymbolTable::SectionTableLine &sctline = symbolTable->sectionTable[currentSection];
	if(arg->addrType == AddressType::IMMED || arg->addrType == AddressType::MEMDIR){
		if(arg->argType == ArgumentType::SYM){
			SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
		}
		if(arg->argType == ArgumentType::LITERAL){
			if(canFitInDispl(arg->intVal, 0))
				return arg->intVal;
			else
				cout<<"LITERAL POOL IS NEEDED\n";
		}
	}
	if(arg->addrType == AddressType::REGINDOFF){
		if(arg->argType == ArgumentType::REGPLUSSYM){
			SymbolTable::SymbolTableLine &symline = symbolTable->symbolTable[arg->stringVal];
			reloTable->handleNewReloLine(locationCounter + 2, RelocationTable::RelocationType::R_32, arg->stringVal);
		}
		if(arg->argType == ArgumentType::REGPLUSLIT){
			if(canFitInDispl(arg->intVal, 0))
				return arg->intVal;
			else
				cout<<"LITERAL POOL IS NEEDED\n";
		}
	}
	return 0;
}