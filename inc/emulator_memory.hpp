#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

#define ADDRESS_SPACE 1UL<<32

class Memory{
public:
	static uint8_t *memory;

	static void loadIntoMemory(ifstream& in);

	static vector<uint8_t> getBytesFromLine(string line);

	static vector<uint8_t> readLine(size_t address);

	static void write4Bytes(size_t address, size_t value);

	static void write4BytesLittleEndian(size_t address, size_t value);

	static size_t read4Bytes(size_t address);

	static size_t read4BytesLittleEndian(size_t address);

	static void printMemory(size_t start, size_t end);

	static vector<uint8_t> getInstructionFromLine(string line);
};