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
		locationCounter += 4;
	}
	cout<<"WORD\n";
}

void SecondPass::handleSkipDirective(AssemblyLine* line){
	locationCounter += line->args[0]->intVal;
	cout<<"SKIP\n";
}

void SecondPass::handleAsciiDirective(AssemblyLine* line){
	cout<<line->args[0]->stringVal<<endl;
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
    locationCounter += 4;
	string content = AssemblyInstruction::getHaltBytes();
    std::cout << "HALT\n";
}

void SecondPass::handleIntInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "INT\n";
}

void SecondPass::handleIretInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "IRET\n";
}

void SecondPass::handleCallInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "CALL\n";
}

void SecondPass::handleRetInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "RET\n";
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
    locationCounter += 4;
    std::cout << "PUSH\n";
}

void SecondPass::handlePopInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "POP\n";
}

void SecondPass::handleXchgInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "XCHG\n";
	string content = AssemblyInstruction::getXchgBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleAddInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "ADD\n";
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleSubInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "SUB\n";
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleMulInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "MUL\n";
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleDivInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "DIV\n";
	string content = AssemblyInstruction::getArithmBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleNotInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "NOT\n";
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleAndInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "AND\n";
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleOrInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "OR\n";
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleXorInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "XOR\n";
	string content = AssemblyInstruction::getLogicBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleShlInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "SHL\n";
	string content = AssemblyInstruction::getShiftBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleShrInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "SHR\n";
	string content = AssemblyInstruction::getShiftBytes(line);
	cout<<content<<endl;
}

void SecondPass::handleLdInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "LD\n";
}

void SecondPass::handleStInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "ST\n";
}

void SecondPass::handleCsrrdInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "CSRRD\n";
}

void SecondPass::handleCsrwrInstruction(AssemblyLine* line) {
    locationCounter += 4;
    std::cout << "CSRWR\n";
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