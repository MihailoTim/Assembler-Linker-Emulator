#include "../h/assemblyFile.hpp"
#include "../h/symbolTable.hpp"
#include <set>

class SecondPass{
public:

    initializer_list<string> instructionsInitializer = {
        "halt", "int", "iret", "call", "ret", "jmp", "beq", "bne", "bgt", "push",
        "pop", "xchg", "add", "sub", "mul", "div", "not", "and", "or", "xor",
        "shl", "shr", "ld", "st", "csrrd", "csrwr"
    };

	initializer_list<string> directivesInitializer = {
		".global", ".extern", ".section", ".word", ".skip", ".ascii", ".equ", ".end"
	};

    set<string> INSTRUCTIONS_SET{instructionsInitializer};
	set<string> DIRECTIVES_SET{directivesInitializer};

	map <string, void (*)(AssemblyLine*)> directiveHandlers {
		{".global", handleGlobalDirective},
		{".extern", handleExternDirective},
		{".section", handleSectionDirective},
		{".word", handleWordDirective},
		{".skip", handleSkipDirective},
		{".ascii", handleAsciiDirective},
		{".equ", handleEquDirective},
		{".end", handleEndDirective}
	};

	map<string, void (*)(AssemblyLine*)> instructionHandlers {
        {"halt", handleHaltInstruction},
        {"int", handleIntInstruction},
        {"iret", handleIretInstruction},
        {"call", handleCallInstruction},
        {"ret", handleRetInstruction},
        {"jmp", handleJmpInstruction},
        {"beq", handleBeqInstruction},
        {"bne", handleBneInstruction},
        {"bgt", handleBgtInstruction},
        {"push", handlePushInstruction},
        {"pop", handlePopInstruction},
        {"xchg", handleXchgInstruction},
        {"add", handleAddInstruction},
        {"sub", handleSubInstruction},
        {"mul", handleMulInstruction},
        {"div", handleDivInstruction},
        {"not", handleNotInstruction},
        {"and", handleAndInstruction},
        {"or", handleOrInstruction},
        {"xor", handleXorInstruction},
        {"shl", handleShlInstruction},
        {"shr", handleShrInstruction},
        {"ld", handleLdInstruction},
        {"st", handleStInstruction},
        {"csrrd", handleCsrrdInstruction},
        {"csrwr", handleCsrwrInstruction}
    };

	enum PassStatus {INACTIVE, RUNNING, FINISHED};

	static SecondPass& getInstance(){
		static SecondPass instance;
		return instance;
	}
	SecondPass(AssemblyFile *f): file(f), status(RUNNING){}

	inline void setFile(AssemblyFile *f){file = f;}

	void start();

	void performBetweenPassCheck();

	void performLineByLine();

	void handleDirective(AssemblyLine *line);

	void handleInstruction(AssemblyLine *line);

    void setSymbolTable(SymbolTable *st){symbolTable = st;}

    static bool canFitInDispl(size_t first, size_t second){return ((int)first - (int)second <= 2<<11) &&  ((int)first-(int)second >= - (2<<11+1));}

	static int handleBranchArgument(Argument *arg);

	static void handleGlobalDirective(AssemblyLine *line);
	static void handleExternDirective(AssemblyLine *line);
	static void handleSectionDirective(AssemblyLine *line);
	static void handleWordDirective(AssemblyLine *line);
	static void handleSkipDirective(AssemblyLine *line);
	static void handleAsciiDirective(AssemblyLine *line);
	static void handleEquDirective(AssemblyLine *line);
	static void handleEndDirective(AssemblyLine *line);

	static void handleHaltInstruction(AssemblyLine* line);
    static void handleIntInstruction(AssemblyLine* line);
    static void handleIretInstruction(AssemblyLine* line);
    static void handleCallInstruction(AssemblyLine* line);
    static void handleRetInstruction(AssemblyLine* line);
    static void handleJmpInstruction(AssemblyLine* line);
    static void handleBeqInstruction(AssemblyLine* line);
    static void handleBneInstruction(AssemblyLine* line);
    static void handleBgtInstruction(AssemblyLine* line);
    static void handlePushInstruction(AssemblyLine* line);
    static void handlePopInstruction(AssemblyLine* line);
    static void handleXchgInstruction(AssemblyLine* line);
    static void handleAddInstruction(AssemblyLine* line);
    static void handleSubInstruction(AssemblyLine* line);
    static void handleMulInstruction(AssemblyLine* line);
    static void handleDivInstruction(AssemblyLine* line);
    static void handleNotInstruction(AssemblyLine* line);
    static void handleAndInstruction(AssemblyLine* line);
    static void handleOrInstruction(AssemblyLine* line);
    static void handleXorInstruction(AssemblyLine* line);
    static void handleShlInstruction(AssemblyLine* line);
    static void handleShrInstruction(AssemblyLine* line);
    static void handleLdInstruction(AssemblyLine* line);
    static void handleStInstruction(AssemblyLine* line);
    static void handleCsrrdInstruction(AssemblyLine* line);
    static void handleCsrwrInstruction(AssemblyLine* line);

	inline PassStatus getStatus(){return status;}

	inline void setStatus(PassStatus s){status = s;}
private:
	AssemblyFile *file;
	static SymbolTable *symbolTable;
	PassStatus status;

	static string currentSection;
	static size_t locationCounter;

	SecondPass(){}
	SecondPass(SecondPass const&);
	void operator=(SecondPass const&);
};