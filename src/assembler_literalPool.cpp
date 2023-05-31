#include "../inc/assembler_literalPool.hpp"
#include "../inc/assembler_assemblyInstruction.hpp"
#include "../inc/assembler_secondPass.hpp"

map<size_t, LiteralPool::LiteralPoolEntry*> LiteralPool::literalPool;
size_t LiteralPool::count = 0;
size_t LiteralPool::startAddress = INT32_MAX;
size_t LiteralPool::currentAddend = 0;
map<size_t, size_t> LiteralPool::forwardBranches;

void LiteralPool::printLiteralPool(){
	for(auto it = literalPool.begin();it!=literalPool.end();it++){
		string content = AssemblyInstruction::get4Bytes(it->second->value);
		cout<<it->first<<": "<<content<<endl;
	}
}

void LiteralPool::handleNewLiteralPoolEntry(size_t locationCounter, int value, RelocationTable::RelocationTableLine* ref){
	count++;
	LiteralPoolEntry* entry = new LiteralPoolEntry(value, ref);
	literalPool.insert(make_pair(locationCounter, entry));
}

void LiteralPool::dumpPool(string &content, size_t &newLocationCounter){
	size_t rememberedLC = newLocationCounter;
	string res="";
	map<size_t, LiteralPoolEntry*> newLiteralPool;
	if(literalPool.size() > 0){
		newLocationCounter += 4;
		for(auto it = literalPool.begin();it!=literalPool.end();it++){
			if(it->first < rememberedLC){
				string literalPoolContent = AssemblyInstruction::get4Bytes(it->second->value);
				res+=literalPoolContent;
				size_t oldLocationCounter = it->first;
				size_t displ = newLocationCounter - oldLocationCounter - 2;
				string byte3 = AssemblyInstruction::getByte((displ >> 8) & 0xF);
				string byte4 = AssemblyInstruction::getByte(displ & 0xFF);
				string replacement = byte3 + byte4;
				content.replace(oldLocationCounter*2+1, 3, replacement.substr(1));
				if(it->second->reference != nullptr){
					it->second->reference->offset = newLocationCounter;
				}
				newLocationCounter+=4;
			}
		}
		size_t delta = newLocationCounter - rememberedLC;
		currentAddend += delta;
		for(auto it = literalPool.begin();it!=literalPool.end();it++){
			if(it->first >= rememberedLC){
				newLiteralPool.insert(make_pair(it->first+delta, it->second));
			}
		}
		string preamble = getLiteralPoolSkipInstruction(delta-4);
		literalPool = newLiteralPool;
		content+=preamble + res;
		updateSymbolTable(rememberedLC);
		updateForwardBranches(content, rememberedLC);
	}
}

void LiteralPool::updateSymbolTable(size_t startingPoint){
	SymbolTable* symtab = SecondPass::symbolTable;
	for(auto it = symtab->symbolTable.begin(); it!=symtab->symbolTable.end();it++){
		if(it->second.value > startingPoint){
			it->second.value += currentAddend;
		}
	}
}

void LiteralPool::updateForwardBranches(string &content, size_t startingPoint){
	for(auto it = forwardBranches.begin(); it!=forwardBranches.end();it++){
		if(it->first + it->second > startingPoint && it->first < startingPoint){
			size_t displ  = it->second + currentAddend;
			string byte3 = AssemblyInstruction::getByte((displ >> 8) & 0xF);
			string byte4 = AssemblyInstruction::getByte(displ & 0xFF);
			string replacement = byte3 + byte4;
			content.replace(it->first*2+1, 3, replacement.substr(1));
		}
	}
	forwardBranches.clear();
}


void LiteralPool::conditionalDumpPool(string &content, size_t &newLocationCounter, size_t &previousLocationCounter){
	if(literalPool.size() > 0 && int(newLocationCounter - literalPool.begin()->first) > int(MAX_POOL_DISPLACEMENT)){
		string addend = content.substr(previousLocationCounter*2);
		content = content.substr(0,previousLocationCounter*2);
		size_t remeberedLC = previousLocationCounter;
		dumpPool(content, previousLocationCounter);
		size_t delta = previousLocationCounter - remeberedLC;
		newLocationCounter += delta;
		content += addend;
	}
}

string LiteralPool::getLiteralPoolSkipInstruction(size_t delta){
	string res = "30f0";
	size_t size = delta;
	string byte3 = AssemblyInstruction::getByte((size >> 8) && 0xF);
	string byte4 = AssemblyInstruction::getByte(size & 0xFF);
	res += byte3 + byte4;
	return res;
}