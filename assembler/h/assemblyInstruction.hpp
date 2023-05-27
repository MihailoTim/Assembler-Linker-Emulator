#include <string>
#include "../h/argument.hpp"
#include "../h/assemblyLine.hpp"
#include "../h/symbolTable.hpp"
#include <map>
using namespace std;

class AssemblyInstruction{
public:
	static string getHaltBytes();
	static string getIntBytes();
	static string getCallBytes(AssemblyLine* line);
	static string getBranchBytes(AssemblyLine* line, size_t displ = 0);
	static string getXchgBytes(AssemblyLine* line);
	static string getArithmBytes(AssemblyLine* line);
	static string getLogicBytes(AssemblyLine* line);
	static string getShiftBytes(AssemblyLine* line);
	static string getStoreBytes(AssemblyLine* line);
	static string getLoadBytes(AssemblyLine* line);

	static string getByte(uint8_t byte);
	static string get4Bytes(int byte1, int byte2, int byte3, int byte4);

	static long getValueOf(Argument *arg);

	static void setSymbolTable(SymbolTable *st){symbolTable = st;}

	static SymbolTable *symbolTable;
};