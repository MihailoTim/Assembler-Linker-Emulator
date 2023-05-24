#include <iostream>
#include "../h/symbolTable.hpp"

class FirstPass{
	size_t lineCount;
	size_t locationCounter;
	SymbolTable& symbolTable;
	FirstPass(): symbolTable(SymbolTable::getInstance()){
		lineCount = 1;
	}
	FirstPass(FirstPass const&);
	void operator=(FirstPass const&);
	
public:
	static FirstPass& getInstance(){
		static FirstPass instance;
		return instance;
	}

	inline void incLineCount(){lineCount++;}
	inline size_t getLineCount(){return lineCount;}
	inline size_t incAndGetLineCount(){return lineCount++;}
	inline void incLocationCounter(size_t add){locationCounter+=add;}
	inline size_t getLocationCounter(){return locationCounter;}
	inline size_t getAndIncLocationCounter(size_t add){return locationCounter+=add;}
	inline void setLocationCounter(size_t val){locationCounter=val;}

	inline void handleGlobalDirective(string symbol){symbolTable.handleGlobalDirective(symbol);}

	inline void handleExternDirective(string symbol){symbolTable.handleExternDirective(symbol);}

	inline void handleSectionDirective(string symbol){symbolTable.handleSectionDirective(symbol, locationCounter);setLocationCounter(0); }

	inline void handleEndDirective(){symbolTable.handleEndDirective(locationCounter);setLocationCounter(0);}

	inline void handleLabel(string symbol){symbolTable.handleLabel(symbol, locationCounter);}

	inline void handleAsciiDirective(string str){}

	inline void printSymbolTable(){symbolTable.printSymbolTable();}

	inline void printSectionTable(){symbolTable.printSectionTable();}



};