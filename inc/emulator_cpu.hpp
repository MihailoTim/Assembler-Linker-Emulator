#include <iostream>
#include <vector>

#define CAUSE_OPCODE 1
#define CAUSE_TIMER 2
#define CAUSE_TERMINAL 3
#define CAUSE_INT 4
#define CAUSE_HALT 5
#define STACK_BASE 0xFFFFFF00
using namespace std;

class CPU{
public:
	static vector<size_t> r;
	static vector<size_t> sreg;

	static size_t &pc;
	static size_t &sp;

	static size_t &status;
	static size_t &handler;
	static size_t &cause;

	static void printRegisterFile();

	static void emulate(size_t startAddr);

	static size_t emulateInstruction(vector<uint8_t> bytes);

    static size_t emulateHalt(const vector<uint8_t>& bytes);
    static size_t emulateInt(const vector<uint8_t>& bytes);
    static size_t emulateIret(const vector<uint8_t>& bytes);
    static size_t emulateCall(const vector<uint8_t>& bytes);
	static size_t emulateBranch(const vector<uint8_t>& bytes);
    static size_t emulateRet(const vector<uint8_t>& bytes);
	static size_t emulateXchg(const vector<uint8_t>& bytes);
	static size_t emulateArithmetic(const vector<uint8_t>& bytes);
	static size_t emulateLogic(const vector<uint8_t>& bytes);
	static size_t emulateShift(const vector<uint8_t>& bytes);
    static size_t emulateLd(const vector<uint8_t>& bytes);
    static size_t emulateSt(const vector<uint8_t>& bytes);

	static size_t executePush(size_t value);
	static size_t executePop(size_t value);
};