#include "../inc/assembler_assemblyInstruction.hpp"
#include <sstream>
#include <iomanip>

string AssemblyInstruction::getByte(uint8_t byte){
	string result="";
	std::stringstream sstream;
	sstream << setw(2) << setfill('0') << std::hex << int(byte);
	result = sstream.str();
	return result;
}

string AssemblyInstruction::get4Bytes(int bytes){
    uint8_t byte1 = (bytes >> 24) & 0xFF;
    uint8_t byte2 = (bytes >> 16) & 0xFF;
    uint8_t byte3 = (bytes >> 8) & 0xFF;
    uint8_t byte4 = bytes & 0xFF;

	return getByte(byte1) + getByte(byte2) + getByte(byte3) + getByte(byte4);
}

string AssemblyInstruction::get4Bytes(int byte1, int byte2, int byte3, int byte4){
	return getByte(byte1) + getByte(byte2) + getByte(byte3) + getByte(byte4);
}

string AssemblyInstruction::getHaltBytes(AssemblyLine *line, size_t displ){
	return get4Bytes(0, 0, 0, 0);
}

string AssemblyInstruction::getIntBytes(AssemblyLine *line, size_t displ){
	return get4Bytes(0x10, 0, 0, 0);
}

string AssemblyInstruction::getCallBytes(AssemblyLine* line, size_t displ){
	int byte1 = 0x21;
	int byte2 = 15 << 4;
	int byte3 = (displ >> 8) & 0xF;
	int byte4 = displ & 255;
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getBranchBytes(AssemblyLine *line, size_t displ, bool useDispl){
	if(line->mnemonic == "jmp"){
		int byte1 = useDispl ? 0x30 :0x38;
		int byte2 = 15 << 4;
		int byte3 = (displ >> 8) & 0xF;
		int byte4 = displ & 255;
		return get4Bytes(byte1, byte2, byte3, byte4);
	}

	int byte1 = 0;
	int byte2 = 15 << 4 | line->args[0]->intVal;
	int byte3 = line->args[1]->intVal << 4 | (displ >> 8) & 0xF;
	int byte4 = displ & 255;


	if(line->mnemonic == "beq"){
		byte1 =useDispl ? 0x31 : 0x39;
	}
	if(line->mnemonic == "bne"){
		byte1 =useDispl ? 0x32 : 0x3A;
	}
	if(line->mnemonic == "bgt"){
		byte1 =useDispl ? 0x33 : 0x3B;
	}
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getXchgBytes(AssemblyLine *line, size_t displ){
	int byte1 = 0x40;
	int byte2 = line->args[0]->intVal;
	int byte3 = line->args[1]->intVal << 4;

	return get4Bytes(byte1, byte2, byte3, 0);
}

string AssemblyInstruction::getArithmBytes(AssemblyLine* line, size_t displ){
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

string AssemblyInstruction::getLogicBytes(AssemblyLine *line, size_t displ){
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

string AssemblyInstruction::getShiftBytes(AssemblyLine *line, size_t displ){
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

string AssemblyInstruction::getLoadBytes(AssemblyLine *line, size_t displ){
	int byte1;
	int byte2;
	int byte3;
	int byte4;
	if(line->mnemonic == "csrrd"){
		byte1 = 0x90;
		byte2 = line->args[1]->intVal << 4 | line->args[0]->intVal;
		byte3 = 0;
		byte4 = 0;
	}
	if(line->mnemonic == "csrwr"){
		byte1 = 0x94;
		byte2 = line->args[1]->intVal << 4 | line->args[0]->intVal;
		byte3 = 0;
		byte4 = 0;
	}
	if(line->mnemonic == "ld"){
		if(line->args[0]->addrType == AddressType::IMMED || line->args[0]->addrType == AddressType::MEMDIR){
			byte1 = 0x92;
			byte2 = line->args[1]->intVal << 4 | 0xF;
			byte3 = (displ >> 8) & 0xF;
			byte4 = displ & 255;
			return get4Bytes(byte1, byte2, byte3, byte4);
		}
		if(line->args[0]->addrType == AddressType::REGDIR){
			byte1 = 0x91;
			byte2 = line->args[1]->intVal << 4 | line->args[0]->intVal;
			byte3 = 0;
			byte4 = 0;
			return get4Bytes(byte1, byte2, byte3, byte4);
		}
		if(line->args[0]->addrType == AddressType::REGIND){
			byte1 = 0x92;
			byte2 = line->args[1]->intVal << 4 | line->args[0]->intVal;
			byte3 = 0;
			byte4 = 0;
			return get4Bytes(byte1, byte2, byte3, byte4);
		}
		if(line->args[0]->addrType == AddressType::REGINDOFF){
			byte1 = 0x92;
			byte2 = line->args[1]->intVal << 4 | line->args[0]->intVal;
			byte3 = (displ >> 8) & 0xF;
			byte4 = displ & 255;
			return get4Bytes(byte1, byte2, byte3, byte4);
		}
	}
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getStoreBytes(AssemblyLine *line, size_t displ){
	int byte1;
	int byte2;
	int byte3;
	int byte4;
	if(line->args[1]->addrType == AddressType::IMMED || line->args[1]->addrType == AddressType::MEMDIR){
		byte1 = 0x82;
		byte2 = 0xF << 4;
		byte3 = line->args[0]->intVal << 4 | ((displ >> 8) & 0xF);
		byte4 = displ & 0xFF;
		return get4Bytes(byte1, byte2, byte3, byte4);
	}
	if(line->args[1]->addrType == AddressType::REGDIR){
		byte1 = 0x91;
		byte2 = line->args[1]->intVal << 4 | line->args[0]->intVal;
		byte3 = 0;
		byte4 = 0;
		return get4Bytes(byte1, byte2, byte3, byte4);
	}
	if(line->args[1]->addrType == AddressType::REGIND){
		byte1 = 0x80;
		byte2 = line->args[1]->intVal << 4 | 0;
		byte3 = line->args[0]->intVal << 4 | 0;
		byte4 = 0;
		return get4Bytes(byte1, byte2, byte3, byte4);
	}
	if(line->args[1]->addrType == AddressType::REGINDOFF){
		byte1 = 0x80;
		byte2 = line->args[1]->intVal << 4 | 0;
		byte3 = line->args[0]->intVal << 4 | (displ >> 8) & 0xF;
		byte4 = displ & 255;
		return get4Bytes(byte1, byte2, byte3, byte4);
	}
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getPopBytes(AssemblyLine *line, size_t displ){
	//REGIND LOAD
	int byte1 = 0x92;
	int byte2 = line->args[0]->intVal << 4 | 0xE;
	int byte3 = 0;
	int byte4 = 4;
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getPushBytes(AssemblyLine *line, size_t displ){
	//REGIND STORE
	int byte1 = 0x81;
	int byte2 = 0xE << 4 | 0;
	int byte3 = ((-4) >> 8) & 0xF ;
	int byte4 = (-4) & 0xFF;
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getRetBytes(AssemblyLine *line, size_t displ){
	//POP PC
	int byte1 = 0x92;
	int byte2 = 0xF << 4 | 0xE;
	int byte3 = 0;
	int byte4 = 4;
	return get4Bytes(byte1, byte2, byte3, byte4);
}

string AssemblyInstruction::getIretBytes(AssemblyLine *line, size_t displ){
	// 0x96, 0, sp, 0, 4
	//0x93, ld pc, sp += 8
	//POP PC, 
	//CSRRD mem[sp], status, sp = sp +4	
	int byte1 = 0x96;
	int byte2 = 0 << 4 | 0xE;
	int byte3 = 0;
	int byte4 = 4;
	string row1 = get4Bytes(byte1, byte2, byte3, byte4);

	byte1 = 0x93;
	byte2 = 0xF <<4 | 0xE;
	byte3 = 0;
	byte4 = 4;
	string row2 = get4Bytes(byte1, byte2, byte3, byte4);
	return row1 + row2;
}

long AssemblyInstruction::getValueOf(Argument *arg){
	return arg->intVal;
}