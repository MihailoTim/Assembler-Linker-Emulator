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

	static const size_t MAX_POOL_DISPLACEMENT = 1<<5;

	static map<size_t, LiteralPoolEntry*> literalPool;

	static size_t startAddress;

	static size_t count;

	static size_t currentAddend;

	static void handleNewLiteralPoolEntry(size_t locationCounter, int value, RelocationTable::RelocationTableLine* line);

	static void printLiteralPool();

	static string getLiteralPoolSkipInstruction();

	static inline void changeSection(){currentAddend = 0;}

	static string dumpPool(string &content, size_t &locationCounter);

	static string conditionalDumpPool(string &content, size_t &locationCounter, size_t &previousLocationCounter);
};