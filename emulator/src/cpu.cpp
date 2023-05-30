#include "../inc/cpu.hpp"
#include "../inc/memory.hpp"
#include <iomanip>
#include <algorithm>

vector<size_t> CPU::r(16);

size_t& CPU::sp = r[14];

size_t& CPU::pc = r[15];

vector<size_t> CPU::sreg(3);
size_t& CPU::status = sreg[0];
size_t& CPU::handler = sreg[1];
size_t& CPU::cause = sreg[2];

void CPU::emulate(size_t startdAddr){
	pc = startdAddr;
	sp = STACK_BASE;
	size_t ret = 0;
	while(ret == 0){
		vector<uint8_t> line = Memory::readLine(pc);
		reverse(line.begin(), line.end());
		for(auto byte : line){
			cout<<hex<<setw(2)<<setfill('0')<<int(byte)<<" ";
		}
		cout<<endl;
		ret = emulateInstruction(line);
		pc+=4;
	}
}

size_t CPU::emulateInstruction(vector<uint8_t> bytes){
	switch(bytes[0] >> 4){
		case 0x0 : return emulateHalt(bytes);
		case 0x1 : return emulateInt(bytes);
		case 0x2 : return emulateCall(bytes);
	}
	return 0;
}

size_t CPU::emulateHalt(const vector<uint8_t>& bytes) {
    if(bytes[0] == 0)
		return CAUSE_HALT;
    return CAUSE_OPCODE;
}

size_t CPU::emulateInt(const vector<uint8_t>& bytes) {
    if(bytes[0] == 1<<4)
		return CAUSE_INT;
    return CAUSE_OPCODE;
}

size_t CPU::emulateIret(const vector<uint8_t>& bytes) {
    // Implementation for emulateIret
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateCall(const vector<uint8_t>& bytes) {
    if(bytes[0] == 0x20){
		size_t reg1 = bytes[1] >> 4 & 0xF;
		size_t reg2 = bytes[1] & 0xF;
		size_t displ = size_t(bytes[2] & 0xF )  << 4 + size_t(bytes[3]);
		sp-=4;
		Memory::write4Bytes(sp, pc);
		pc = reg1 + reg2 + displ;
	}
	if(bytes[0] == 0x21){
		size_t reg1 = bytes[1] >> 4 & 0xF;
		size_t reg2 = bytes[1] & 0xF;
		size_t displ = size_t(bytes[2] & 0xF )  << 4 + size_t(bytes[3]);
		sp-=4;
		Memory::write4Bytes(sp, pc);
		pc = Memory::memory[reg1 + reg2 + displ];
	}
    return 0; // Placeholder return value
}

size_t CPU::emulateRet(const vector<uint8_t>& bytes) {
    // Implementation for emulateRet
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateJmp(const vector<uint8_t>& bytes) {
    // Implementation for emulateJmp
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateBeq(const vector<uint8_t>& bytes) {
    // Implementation for emulateBeq
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateBne(const vector<uint8_t>& bytes) {
    // Implementation for emulateBne
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateBgt(const vector<uint8_t>& bytes) {
    // Implementation for emulateBgt
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulatePush(const vector<uint8_t>& bytes) {
    // Implementation for emulatePush
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulatePop(const vector<uint8_t>& bytes) {
    // Implementation for emulatePop
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateXchg(const vector<uint8_t>& bytes) {
    // Implementation for emulateXchg
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateAdd(const vector<uint8_t>& bytes) {
    // Implementation for emulateAdd
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateSub(const vector<uint8_t>& bytes) {
    // Implementation for emulateSub
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateMul(const vector<uint8_t>& bytes) {
    // Implementation for emulateMul
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateDiv(const vector<uint8_t>& bytes) {
    // Implementation for emulateDiv
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateNot(const vector<uint8_t>& bytes) {
    // Implementation for emulateNot
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateAnd(const vector<uint8_t>& bytes) {
    // Implementation for emulateAnd
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateOr(const vector<uint8_t>& bytes) {
    // Implementation for emulateOr
    // ...
    return 0; //
}

size_t CPU::emulateXor(const vector<uint8_t>& bytes) {
    // Implementation for emulateXor
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateShl(const vector<uint8_t>& bytes) {
    // Implementation for emulateShl
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateShr(const vector<uint8_t>& bytes) {
    // Implementation for emulateShr
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateLd(const vector<uint8_t>& bytes) {
    // Implementation for emulateLd
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateSt(const vector<uint8_t>& bytes) {
    // Implementation for emulateSt
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateCsrrd(const vector<uint8_t>& bytes) {
    // Implementation for emulateCsrrd
    // ...
    return 0; // Placeholder return value
}

size_t CPU::emulateCsrwr(const vector<uint8_t>& bytes) {
    // Implementation for emulateCsrwr
    // ...
    return 0; // Placeholder return value
}