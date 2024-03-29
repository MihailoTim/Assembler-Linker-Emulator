#include <iostream>
#include "../inc/emulator_reader.hpp"
#include "../inc/emulator_memory.hpp"
#include "../inc/emulator_cpu.hpp"
#include "../inc/emulator_exceptions.hpp"

using namespace std;

int main(int argc, char** argv){
	string filename = *new string(argv[1]);
	ifstream in(filename);

	if(!in.is_open()){
		throw new Exception("Couldn't open file");
	}

	try{
		Memory::loadIntoMemory(in);
	}
	catch(exception e){
		throw new Exception("Can't parse input file\n");
	}

	CPU::emulate(0x40000000);

	cout<<"\n==============================================================\n";
	cout<<"Emulated process executed halt instruction\n";
	cout<<"Emulated process state:\n";

	CPU::printRegisterFile();

	Memory::printMemory(0x70, 0x90);

	// Memory::printMemory(CPU::sp-0x20, CPU::sp+20);

}