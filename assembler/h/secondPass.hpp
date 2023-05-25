#include "../h/assemblyFile.hpp"
#include "../h/symbolTable.hpp"

class SecondPass{
public:
	enum PassStatus {INACTIVE, RUNNING, FINISHED};

	static SecondPass& getInstance(){
		static SecondPass instance;
		return instance;
	}
	SecondPass(AssemblyFile *f): file(f), symbolTable(SymbolTable::getInstance()), status(RUNNING){}

	inline void setFile(AssemblyFile *f){file = f;}

	void start();

	void performBetweenPassCheck();

	inline PassStatus getStatus(){return status;}

	inline void setStatus(PassStatus s){status = s;}
private:
	AssemblyFile *file;
	SymbolTable &symbolTable;
	PassStatus status;

	SecondPass(): symbolTable(SymbolTable::getInstance()){}
	SecondPass(SecondPass const&);
	void operator=(SecondPass const&);
};