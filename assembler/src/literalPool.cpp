#include "../h/literalPool.hpp"
#include "../h/assemblyInstruction.hpp"
#include "../h/secondPass.hpp"

map<size_t, LiteralPool::LiteralPoolEntry*> LiteralPool::literalPool;
size_t LiteralPool::count = 0;
size_t LiteralPool::startAddress = INT32_MAX;
size_t LiteralPool::currentAddend = 0;

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

string LiteralPool::dumpPool(string &content, size_t &newLocationCounter){
	string res = getLiteralPoolSkipInstruction();
	size_t rememberedLC = newLocationCounter;
	newLocationCounter += 4;
	for(auto it = literalPool.begin();it!=literalPool.end();it++){
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
		content+=res;
		res = "";
	}
	size_t delta = newLocationCounter - rememberedLC;
	currentAddend += delta;
	SymbolTable* symtab = SecondPass::symbolTable;
	for(auto it = symtab->symbolTable.begin(); it!=symtab->symbolTable.end();it++){
		if(it->second.value > rememberedLC){
			it->second.value += currentAddend;
		}
	}
	RelocationTable* reloTable = SecondPass::reloTable;
	for(auto it = reloTable->reloTable.rbegin(); it!=reloTable->reloTable.rend();it++){
		cout<<it->first<<" "<<rememberedLC<<endl;
		if(it->first > rememberedLC){
			cout<<it->first+delta << " ";
			it->second->printReloTableLine();
			cout<<endl;
		}
	}
	literalPool.clear();
	return res;
}

string LiteralPool::conditionalDumpPool(string &content, size_t &newLocationCounter, size_t &previousLocationCounter){
	if(literalPool.size() > 0 && newLocationCounter - literalPool.begin()->first > MAX_POOL_DISPLACEMENT){
		string addend = content.substr(previousLocationCounter*2);
		content = content.substr(0,previousLocationCounter*2);
		size_t remeberedLC = previousLocationCounter;
		dumpPool(content, previousLocationCounter);
		size_t delta = previousLocationCounter - remeberedLC;
		cout<<"DELTA: "<<delta<<endl;
		newLocationCounter += delta;
		content += addend;
	}
}

string LiteralPool::getLiteralPoolSkipInstruction(){
	string res = "eeee";
	size_t size = literalPool.size() * 4;
	string byte3 = AssemblyInstruction::getByte((size >> 8) && 0xF);
	string byte4 = AssemblyInstruction::getByte(size & 0xFF);
	res += byte3 + byte4;
	return res;
}