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


	static void emulate(size_t startAddr);

	static size_t emulateInstruction(vector<uint8_t> bytes);

    static size_t emulateHalt(const vector<uint8_t>& bytes);
    static size_t emulateInt(const vector<uint8_t>& bytes);
    static size_t emulateIret(const vector<uint8_t>& bytes);
    static size_t emulateCall(const vector<uint8_t>& bytes);
    static size_t emulateRet(const vector<uint8_t>& bytes);
    static size_t emulateJmp(const vector<uint8_t>& bytes);
    static size_t emulateBeq(const vector<uint8_t>& bytes);
    static size_t emulateBne(const vector<uint8_t>& bytes);
    static size_t emulateBgt(const vector<uint8_t>& bytes);
    static size_t emulatePush(const vector<uint8_t>& bytes);
    static size_t emulatePop(const vector<uint8_t>& bytes);
    static size_t emulateXchg(const vector<uint8_t>& bytes);
    static size_t emulateAdd(const vector<uint8_t>& bytes);
    static size_t emulateSub(const vector<uint8_t>& bytes);
    static size_t emulateMul(const vector<uint8_t>& bytes);
    static size_t emulateDiv(const vector<uint8_t>& bytes);
    static size_t emulateNot(const vector<uint8_t>& bytes);
    static size_t emulateAnd(const vector<uint8_t>& bytes);
    static size_t emulateOr(const vector<uint8_t>& bytes);
    static size_t emulateXor(const vector<uint8_t>& bytes);
    static size_t emulateShl(const vector<uint8_t>& bytes);
    static size_t emulateShr(const vector<uint8_t>& bytes);
    static size_t emulateLd(const vector<uint8_t>& bytes);
    static size_t emulateSt(const vector<uint8_t>& bytes);
    static size_t emulateCsrrd(const vector<uint8_t>& bytes);
    static size_t emulateCsrwr(const vector<uint8_t>& bytes);
};