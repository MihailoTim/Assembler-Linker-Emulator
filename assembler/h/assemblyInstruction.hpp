#include <string>
#include "../h/argument.hpp"
#include "../h/assemblyLine.hpp"
#include "../h/symbolTable.hpp"
#include <map>
using namespace std;

class AssemblyInstruction{
public:
	static string getHaltBytes(AssemblyLine *line, size_t displ = 0);
	static string getIntBytes(AssemblyLine *line, size_t displ = 0);
	static string getCallBytes(AssemblyLine* line, size_t displ = 0);
	static string getBranchBytes(AssemblyLine* line, size_t displ = 0);
	static string getXchgBytes(AssemblyLine* line, size_t displ = 0);
	static string getArithmBytes(AssemblyLine* line, size_t displ = 0);
	static string getLogicBytes(AssemblyLine* line, size_t displ = 0);
	static string getShiftBytes(AssemblyLine* line, size_t displ = 0);
	static string getStoreBytes(AssemblyLine* line, size_t displ = 0);
	static string getLoadBytes(AssemblyLine* line, size_t displ = 0);
	static string getPopBytes(AssemblyLine* line, size_t displ = 0);
	static string getPushBytes(AssemblyLine* line, size_t displ = 0);
	static string getRetBytes(AssemblyLine* line, size_t displ = 0);
	static string getIretBytes(AssemblyLine* line, size_t displ = 0);	

	static string getByte(uint8_t byte);
	static string get4Bytes(int bytes);
	static string get4Bytes(int byte1, int byte2, int byte3, int byte4);

	static long getValueOf(Argument *arg);

	static void setSymbolTable(SymbolTable *st){symbolTable = st;}

	static SymbolTable *symbolTable;
};