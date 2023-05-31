#include "../inc/emulator_memory.hpp"
#include <iomanip>

uint8_t* Memory::memory = new uint8_t[ADDRESS_SPACE];

void Memory::loadIntoMemory(ifstream& in){
	string line;
	vector<string> result;
	string strippedLine;

	while(getline(in, line)){
		// try{
			size_t address = stoi(line.substr(0,8), nullptr, 16);
			strippedLine = line.substr(10);
			size_t pos = strippedLine.find(" ");
			while (pos != std::string::npos) {
				strippedLine.replace(pos, 1, "");
				pos = strippedLine.find(" ", pos);
			}
			strippedLine = strippedLine.substr(0,16);
			vector<uint8_t> bytes = getBytesFromLine(strippedLine);

			for(uint8_t byte : bytes){
				memory[address++] = byte;
			}
		// }
		// catch(exception e){
		// 	cout<<"Invalid input file format\n";
		// 	exit(-1);
		// }
	}
}

vector<uint8_t> Memory::getBytesFromLine(string line){
	vector<uint8_t> bytes;
	for(int i=0;i<8;i++){
		uint8_t byte = stoi(line.substr(i*2,2), nullptr, 16);
		cout<<int(byte)<<" ";
		bytes.push_back(byte);
	}
	cout<<endl;
	return bytes;
}

vector<uint8_t> Memory::getInstructionFromLine(string line){
	vector<uint8_t> bytes;
	for(int i=3;i>=0;i--){
		uint8_t byte = stoi(line.substr(i*2,2), nullptr, 16);
		cout<<int(byte)<<" ";
		bytes.push_back(byte);
	}
	for(int i=7;i>=4;i--){
		uint8_t byte = stoi(line.substr(i*2,2), nullptr, 16);
		cout<<int(byte)<<" ";
		bytes.push_back(byte);
	}
	cout<<endl;
	return bytes;
}

void Memory::printMemory(size_t start, size_t end){
	for(size_t j=0, i=(start - start%8); i<(end + 8 - end%8);i++, j++){
		if(j%8 == 0 && j!=0){
			cout<<"\n";
		}
		if(j%8 == 0){
			cout<<hex<<setw(8)<<setfill('0')<<i<<": ";
		}
		cout<<hex<<setw(2)<<setfill('0')<<int(memory[i])<<" ";
	}
	cout<<endl;
}

vector<uint8_t> Memory::readLine(size_t address){
	vector<uint8_t> bytes{memory[address], memory[address+1], memory[address+2], memory[address+3]};
	return bytes;
}

void Memory::write4Bytes(size_t address, size_t value){
	memory[address] = value & 0xFF;
	memory[address+1] = (value >> 8) & 0xFF;
	memory[address+2] = (value >> 16) & 0xFF;
	memory[address+3] = (value >> 24) & 0xFF;
}

size_t Memory::read4Bytes(size_t address){
	size_t byte4 = memory[address];
	size_t byte3 = memory[address+1];
	size_t byte2 = memory[address+2];
	size_t byte1 = memory[address+3];
	return (byte1 << 24) + (byte2 << 16) + (byte3 << 8) + byte4;
}