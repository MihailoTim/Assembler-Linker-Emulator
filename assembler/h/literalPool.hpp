#include <vector>
#include <map>
#include "../h/relocationTable.hpp"
using namespace std;


class LiteralPool{
	struct LiteralPoolEntry{
		int value;
		RelocationTable::RelocationTableLine* reference;

		LiteralPoolEntry(int v, RelocationTable::RelocationTableLine* ref): value(v), reference(ref){}
	};
public:

	static map<size_t, LiteralPoolEntry*> literalPool;


	static size_t startAddress;

	static size_t count;

	static void handleNewLiteralPoolEntry(size_t locationCounter, int value, RelocationTable::RelocationTableLine* line);

	static void printLiteralPool();
};