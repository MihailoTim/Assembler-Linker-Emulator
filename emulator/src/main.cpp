#include <iostream>
#include "../inc/reader.hpp"
#include "../inc/memory.hpp"
#include "../inc/cpu.hpp"

using namespace std;

int main(int argc, char** argv){
	string filename = *new string(argv[1]);
	ifstream in(filename);

	if(!in.is_open()){
		cout<<"Couldn't open file";
	}

	Memory::loadIntoMemory(in);

	Memory::printMemory(0, 200);

	size_t value = Memory::read4Bytes(0);

	CPU::emulate(0);

	CPU::printRegisterFile();

}