#include "../inc/emulator_cpu.hpp"
#include "../inc/emulator_memory.hpp"
#include "../inc/emulator_exceptions.hpp"
#include "../inc/emulator_terminal.hpp"
#include <iomanip>
#include <algorithm>

vector<size_t> CPU::r(16);

size_t& CPU::sp = r[14];

size_t& CPU::pc = r[15];

vector<size_t> CPU::sreg(3);
size_t& CPU::status = sreg[0];
size_t& CPU::handler = sreg[1];
size_t& CPU::cause = sreg[2];

vector<size_t> CPU::interruptQueue;

int interrupt_count = 0;

void CPU::emulate(size_t startdAddr){
	pc = startdAddr;
	sp = STACK_BASE;
	size_t ret = 0;
	Terminal::initialize();
	while(ret == 0){
		r[0] = 0;
		vector<uint8_t> line = Memory::readLine(pc);
		// if(pc == 0xd0){
		// 	cout<<"R1: "<<hex<<r[1]<<endl;
		// }
		// if(pc >= 0xd0 && pc<=0xf0){
		// 	cout<<"R2: "<<hex<<r[2]<<endl;
		// }

		pc+=4;

		ret = emulateInstruction(line);
		Terminal::putChar();


		if(ret == CAUSE_HALT){
			break;
		}
		else if(ret == CAUSE_OPCODE){
			cause = CAUSE_OPCODE;
			printRegisterFile();
			throw new Exception("Opcode not recognized\n");
		}
		else{
			if(isTerminalInterruptEnabled() && isInterruptEnabled()){
				Terminal::getChar();
			}
			if(cause == 3 && isInterruptEnabled() && isTerminalInterruptEnabled() && interruptQueue.size()){
				executePush(status);
				executePush(pc);
				status |= 0x7;
				pc = handler;
				interrupt_count++;
				interruptQueue.clear();
			}
			ret = 0;
		}
	}
}

size_t CPU::emulateInstruction(vector<uint8_t> bytes){
	switch(bytes[0] >> 4){
		case 0x0 : return emulateHalt(bytes);
		case 0x1 : return emulateInt(bytes);
		case 0x2 : return emulateCall(bytes);
		case 0x3 : return emulateBranch(bytes);
		case 0x4 : return emulateXchg(bytes);
		case 0x5 : return emulateArithmetic(bytes);
		case 0x6 : return emulateLogic(bytes);
		case 0x7 : return emulateShift(bytes);
		case 0x8 : return emulateSt(bytes);
		case 0x9 : return emulateLd(bytes);
	}
	return 0;
}

size_t CPU::emulateHalt(const vector<uint8_t>& bytes) {
    if(bytes[0] == 0){
		return CAUSE_HALT;
	}
    return CAUSE_OPCODE;
}

size_t CPU::emulateInt(const vector<uint8_t>& bytes) {
    if(bytes[0] == 1<<4){
		executePush(status);
		executePush(pc);
		cause = 4;
		status = status & (~1UL);
		pc = handler;
		return CAUSE_INT;
	}
    return CAUSE_OPCODE;
}

size_t CPU::emulateCall(const vector<uint8_t>& bytes) {
	size_t reg1 = r[(bytes[1] >> 4) & 0xF];
	size_t reg2 = r[bytes[1] & 0xF];
	int unsignedDispl = (((bytes[2]) & 0xF )  << 8) + (int(bytes[3]));
	int displ = (unsignedDispl & 0x800) ? (unsignedDispl | 0xFFFFF800) : unsignedDispl;
	executePush(pc);

    if(bytes[0] == 0x20){
		pc = reg1 + reg2 + displ;
		// cout<<"CALL TO: "<<hex<<pc<<endl;
		return 0;
	}
	if(bytes[0] == 0x21){
		pc = Memory::read4Bytes(reg1 + reg2 + displ);
		// cout<<"CALL TO: "<<hex<<pc<<endl;
		return 0;
	}
    return CAUSE_OPCODE;
}


size_t CPU::emulateBranch(const vector<uint8_t>& bytes){
	size_t reg1 = r[(bytes[1] >> 4) & 0xF];
	size_t reg2 = r[bytes[1] & 0xF];
	size_t reg3 = r[(bytes[2] >> 4) & 0xF];
	int unsignedDispl = (((bytes[2]) & 0xF )  << 8) + (int(bytes[3]));
	int displ = (unsignedDispl & 0x800) ? (unsignedDispl | 0xFFFFF800) : unsignedDispl;
	switch(bytes[0]){
		case 0x30 : pc = reg1 + displ; break;
		case 0x31 : if(reg2 == reg3) pc = reg1 + displ; break;
		case 0x32 : if(reg2 != reg3) pc = reg1 + displ; break;
		case 0x33 : if(reg2 > reg3) pc = reg1 + displ; break;
		case 0x38 : pc = Memory::read4Bytes(reg1 + displ);
		case 0x39 : if(reg2 == reg3) pc = Memory::read4Bytes(reg1 + displ); break;
		case 0x3a : if(reg2 != reg3) pc = Memory::read4Bytes(reg1 + displ); break;
		case 0x3b : if(reg2 > reg3) pc = Memory::read4Bytes(reg1 + displ); break;
		default: return CAUSE_OPCODE;
	}
	return 0;
}

size_t CPU::emulateXchg(const vector<uint8_t>& bytes) {
    if(bytes[0] == 0x40){
		size_t &reg1 = r[bytes[1] & 0xF];
		size_t &reg2 = r[(bytes[2] >> 4) & 0xF];
		size_t tmp = reg1;
		reg1 = reg2;
		reg2 = tmp;
		return 0;
	}
    return CAUSE_OPCODE;
}

size_t CPU::emulateArithmetic(const vector<uint8_t>& bytes){
	size_t &reg1 = r[(bytes[1] >> 4) & 0xF];
	size_t &reg2 = r[bytes[1] & 0xF];
	size_t &reg3 = r[(bytes[2] >> 4) & 0xF];
	switch(bytes[0]){
		case 0x50: reg1 = reg2 + reg3; break;
		case 0x51: reg1 = reg2 - reg3; break;
		case 0x52: reg1 = int(reg2) * int(reg3); break;
		case 0x53: reg1 = int(reg2) / int(reg3); break;
		default: return CAUSE_OPCODE;
	}
	return 0;
}

size_t CPU::emulateLogic(const vector<uint8_t>& bytes){
	size_t &reg1 = r[(bytes[1] >> 4) & 0xF];
	size_t &reg2 = r[bytes[1] & 0xF];
	size_t &reg3 = r[(bytes[2] >> 4) & 0xF];
	switch(bytes[0]){
		case 0x60: reg1 = ~reg2; break;
		case 0x61: reg1 = reg2 & reg3; break;
		case 0x62: reg1 = reg2 | reg3; break;
		case 0x63: reg1 = reg2 ^ reg3; break;
		default: return CAUSE_OPCODE;
	}
	return 0;
}

size_t CPU::emulateShift(const vector<uint8_t>& bytes){
	size_t &reg1 = r[(bytes[1] >> 4) & 0xF];
	size_t &reg2 = r[bytes[1] & 0xF];
	size_t &reg3 = r[(bytes[2] >> 4) & 0xF];
	switch(bytes[0]){
		case 0x70: reg1 = reg2 << reg3; break;
		case 0x71: reg1 = reg2 >> reg3; break;
		default: return CAUSE_OPCODE;
	}
	return 0;
}

size_t CPU::emulateLd(const vector<uint8_t>& bytes) {
	size_t &reg1 = r[(bytes[1] >> 4) & 0xF];
	size_t &reg2 = r[bytes[1] & 0xF];
	size_t &reg3 = r[(bytes[2] >> 4) & 0xF];
	size_t &sreg1 = sreg[(bytes[1] >> 4) & 0xF];
	size_t &sreg2 = sreg[bytes[1] & 0xF];
	int unsignedDispl = (((bytes[2]) & 0xF )  << 8) + (int(bytes[3]));
	int displ = (unsignedDispl & 0x800) ? (unsignedDispl | 0xFFFFF800) : unsignedDispl;
	switch (bytes[0]){
		case 0x90 : reg1 = sreg2; break;
		case 0x91 : reg1 = reg2 + displ; break;
		case 0x92 : {
		// if(pc >=0xd0 && pc<=0xf0)
		// 	cout<<hex<<reg2<<" "<<reg3<<" "<<displ<<" "<<Memory::read4Bytes(reg2 + reg3 + displ)<<endl;
		 reg1 = Memory::read4Bytes(reg2 + reg3 + displ); break;
		}
		case 0x93 : reg1 = Memory::read4Bytes(reg2); reg2 += displ; break;
		case 0x94 : sreg1 = reg2; break;
		case 0x95 : sreg1 = sreg2 | displ; break;
		case 0x96 : sreg1 = Memory::read4Bytes(reg2 + reg3 + displ); break;
		case 0x97 : sreg1 = Memory::read4Bytes(reg2); reg2 += displ; break;
		default: return CAUSE_OPCODE;
	}
    return 0;
}

size_t CPU::emulateSt(const vector<uint8_t>& bytes) {
	size_t &reg1 = r[(bytes[1] >> 4) & 0xF];
	size_t &reg2 = r[bytes[1] & 0xF];
	size_t &reg3 = r[(bytes[2] >> 4) & 0xF];
	int unsignedDispl = (((bytes[2]) & 0xF )  << 8) + (int(bytes[3]));
	int displ = (unsignedDispl & 0x800) ? (unsignedDispl | 0xFFFFF800) : unsignedDispl;
    switch(bytes[0]){
		case 0x80 : Memory::write4Bytes(reg1+reg2+displ, reg3); break;
		case 0x81 : reg1 += displ; Memory::write4Bytes(reg1, reg3); break;
		case 0x82 : Memory::write4Bytes(Memory::read4Bytes(reg1+reg2+displ), reg3); break;
		default: return CAUSE_OPCODE;
	}
    return 0; 
}

void CPU::printRegisterFile(){
	cout << right;

    for (int i = 0; i < 16; ++i) {
        cout << "r" <<dec<< i << "=" << std::setw(8) << std::setfill('0') << std::hex << (r[i] & 0xFFFFFFFF) << " ";
        if ((i + 1) % 4 == 0) {
            cout << endl;
        }
    }

	cout<<"status="<<status<<endl;
	cout<<"handler="<<handler<<endl;
	cout<<"cause="<<cause<<endl;
}

size_t CPU::executePush(size_t value){
	sp-=4;
	Memory::write4Bytes(sp, value);
	return sp;
}