#include "../h/assemblyInstruction.hpp"

string AssemblyInstruction::getByte(int byte){
	string result="";
	for(int i=0;i<8;i++){
		result = to_string(byte & 1) + result;
		byte>>=1;
	}
	return result;
}

string AssemblyInstruction::get4Bytes(int byte1, int byte2, int byte3, int byte4){
	return getByte(byte1) + " " + getByte(byte2)  + " " + getByte(byte3) + " " + getByte(byte4);
}

string AssemblyInstruction::getHaltBytes(){
	return get4Bytes(0, 0, 0, 0);
}

string AssemblyInstruction::getIntBytes(){
	return get4Bytes(16, 0, 0, 0);
}

string AssemblyInstruction::getCallBytes(AssemblyLine* line){
	return get4Bytes(32, 0, 0, 0);
}

string AssemblyInstruction::getBranchBytes(AssemblyLine *line){
	if(line->mnemonic == "jmp"){
		int byte1 = 48;
		int byte2 = 15 << 4;
		return get4Bytes(byte1, byte2, 0, 0);
	}

	int byte1 = 0;
	int byte2 = 15 << 4 | line->args[0]->intVal;
	int byte3 = line->args[1]->intVal << 4;


	if(line->mnemonic == "beq"){
		byte1 = 49;
	}
	if(line->mnemonic == "bne"){
		byte1 = 50;
	}
	if(line->mnemonic == "bgt"){
		byte1 = 51;
	}
	return get4Bytes(byte1, byte2, byte3, 0);
}

string AssemblyInstruction::getXchgBytes(AssemblyLine *line){
	int byte1 = 64;
	int byte2 = line->args[0]->intVal;
	int byte3 = line->args[1]->intVal << 4;

	return get4Bytes(byte1, byte2, byte3, 0);
}

string AssemblyInstruction::getArithmBytes(AssemblyLine* line){
	int byte1;
	int byte2 = line->args[1]->intVal << 4 | line->args[1]->intVal;
	int byte3 = line->args[0]->intVal << 4;
	if(line->mnemonic == "add"){
		byte1 = 65;
	}
	if(line->mnemonic == "sub"){
		byte1 = 66;
	}
	if(line->mnemonic == "mul"){
		byte1 = 67;
	}
	if(line->mnemonic == "div"){
		byte1 = 68;
	}
	return get4Bytes(byte1, byte2, byte3, 0) ;
}

string AssemblyInstruction::getLogicBytes(AssemblyLine *line){
	if(line->mnemonic == "not"){
		int byte1 = 96;
		int byte2 = line->args[0]->intVal << 4 | line->args[0]->intVal;
		return get4Bytes(byte1, byte2, 0, 0) ;
	}
	int byte1;
	int byte2 = line->args[1]->intVal << 4 | line->args[1]->intVal;
	int byte3 = line->args[0]->intVal << 4;
	if(line->mnemonic == "and"){
		byte1 = 97;
	}
	if(line->mnemonic == "or"){
		byte1 = 98;
	}
	if(line->mnemonic == "xor"){
		byte1 = 99;
	}
	return get4Bytes(byte1, byte2, byte3, 0) ;
}

string AssemblyInstruction::getShiftBytes(AssemblyLine *line){
	int byte1;
	int byte2 = line->args[1]->intVal << 4 | line->args[1]->intVal;
	int byte3 = line->args[0]->intVal << 4;

	if(line->mnemonic == "shl"){
		byte1 = 112;
	}
	
	if(line->mnemonic == "shr"){
		byte1 = 113;
	}

	return get4Bytes(byte1, byte2, byte3, 0);
}

long AssemblyInstruction::getValueOf(Argument *arg){
	return arg->intVal;
}