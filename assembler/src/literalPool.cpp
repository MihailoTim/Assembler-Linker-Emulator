#include "../h/literalPool.hpp"
#include "../h/assemblyInstruction.hpp"

map<size_t, LiteralPool::LiteralPoolEntry*> LiteralPool::literalPool;
size_t LiteralPool::count = 0;
size_t LiteralPool::startAddress = INT32_MAX;

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