#include <vector>
#include <map>
#include "../inc/assembler_relocationTable.hpp"
using namespace std;

#ifndef _LITERAL_POOL_H
#define _LITERAL_POOL_H

class LiteralPool{
	struct LiteralPoolEntry{
		int value;
		RelocationTable::RelocationTableLine* reference;

		LiteralPoolEntry(int v, RelocationTable::RelocationTableLine* ref): value(v), reference(ref){}
	};
public:

	static const size_t MAX_POOL_DISPLACEMENT = 1<<8;

	static map<size_t, LiteralPoolEntry*> literalPool;

	static map<size_t, size_t> forwardBranches;

	static size_t startAddress;

	static size_t count;

	static size_t currentAddend;

	static void handleNewLiteralPoolEntry(size_t locationCounter, int value, RelocationTable::RelocationTableLine* line);

	static void printLiteralPool();

	static void handleForwardBranch(size_t locationCounter, size_t oldValue){forwardBranches.insert(make_pair(locationCounter, oldValue));}

	static string getLiteralPoolSkipInstruction(size_t delta);

	static inline void changeSection(){currentAddend = 0;}

	static void updateSymbolTable(size_t startingPoint);

	static void updateForwardBranches(string &content, size_t startingPoint);

	static void dumpPool(string &content, size_t &locationCounter);

	static void conditionalDumpPool(string &content, size_t &locationCounter, size_t &previousLocationCounter);
};

#endif