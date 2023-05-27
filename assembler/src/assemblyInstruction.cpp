#include "../h/assemblyInstruction.hpp"
#include <sstream>
#include <iomanip>

string AssemblyInstruction::getByte(uint8_t byte){
	string result="";
	std::stringstream sstream;
	sstream << setw(2) << setfill('0') << std::hex << int(byte);
	result = sstream.str();
	return result;
}

string AssemblyInstruction::get4Bytes(int byte1, int byte2, int byte3, int byte4){
	return getByte(byte1) + " " + getByte(byte2)  + " " + getByte(byte3) + " " + getByte(byte4);
}

string AssemblyInstruction::getHaltBytes(){
	return get4Bytes(0, 0, 0, 0);
}

string AssemblyInstruction::getIntBytes(){
	return get4Bytes(0x10, 0, 0, 0);
}

string AssemblyInstruction::getCallBytes(AssemblyLine* line){
	return get4Bytes(0x20, 0, 0, 0);
}

string AssemblyInstruction::getBranchBytes(AssemblyLine *line, size_t displ){
	if(line->mnemonic == "jmp"){
		int byte1 = 0x30;
		int byte2 = 15 << 4;
		int byte3 = displ >> 8;
		int byte4 = displ & 255;
		return get4Bytes(byte1, byte2, byte3, byte4);
	}

	int byte1 = 0;
	int byte2 = 15 << 4 | line->args[0]->intVal;
	int byte3 = line->args[1]->intVal << 4 | displ >> 8;
	int byte4 = displ & 255;


	if(line->mnemonic == "beq"){
		byte1 = 0x31;
	}
	if(line->mnemonic == "bne"){
		byte1 = 0x32;
	}
	if(line->mnemonic == "bgt"){
		byte1 = 0x33;
	}
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getXchgBytes(AssemblyLine *line){
	int byte1 = 0x40;
	int byte2 = line->args[0]->intVal;
	int byte3 = line->args[1]->intVal << 4;

	return get4Bytes(byte1, byte2, byte3, 0);
}

string AssemblyInstruction::getArithmBytes(AssemblyLine* line){
	int byte1;
	int byte2 = line->args[1]->intVal << 4 | line->args[1]->intVal;
	int byte3 = line->args[0]->intVal << 4;
	if(line->mnemonic == "add"){
		byte1 = 0x50;
	}
	if(line->mnemonic == "sub"){
		byte1 = 0x51;
	}
	if(line->mnemonic == "mul"){
		byte1 = 0x52;
	}
	if(line->mnemonic == "div"){
		byte1 = 0x53;
	}
	return get4Bytes(byte1, byte2, byte3, 0) ;
}

string AssemblyInstruction::getLogicBytes(AssemblyLine *line){
	if(line->mnemonic == "not"){
		int byte1 = 0x60;
		int byte2 = line->args[0]->intVal << 4 | line->args[0]->intVal;
		return get4Bytes(byte1, byte2, 0, 0) ;
	}
	int byte1;
	int byte2 = line->args[1]->intVal << 4 | line->args[1]->intVal;
	int byte3 = line->args[0]->intVal << 4;
	if(line->mnemonic == "and"){
		byte1 = 0x61;
	}
	if(line->mnemonic == "or"){
		byte1 = 0x62;
	}
	if(line->mnemonic == "xor"){
		byte1 = 0x63;
	}
	return get4Bytes(byte1, byte2, byte3, 0) ;
}

string AssemblyInstruction::getShiftBytes(AssemblyLine *line){
	int byte1;
	int byte2 = line->args[1]->intVal << 4 | line->args[1]->intVal;
	int byte3 = line->args[0]->intVal << 4;

	if(line->mnemonic == "shl"){
		byte1 = 0x70;
	}
	
	if(line->mnemonic == "shr"){
		byte1 = 0x71;
	}

	return get4Bytes(byte1, byte2, byte3, 0);
}

long AssemblyInstruction::getValueOf(Argument *arg){
	return arg->intVal;
}