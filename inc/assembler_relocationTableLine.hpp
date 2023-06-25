	#include <iostream>
  using namespace std;

  #ifndef _RELOCATION_TABLE_LINE_H
  #define _RELOCATION_TABLE_LINE_H
  
  struct RelocationTableLine{
		enum RelocationType{R_32, R_PC32};
		static size_t globalId;
		size_t id;
		size_t offset;
		RelocationType type;
		size_t referencedSymbol;
		string relocatedSymbol;
		long addend;
		RelocationTableLine(size_t o, RelocationType rt, size_t s, long a, string reloSymbol): offset(o), type(rt), referencedSymbol(s), addend(a), relocatedSymbol(reloSymbol){id = globalId++;}

		void printReloTableLine(){
			cout << offset << " " << (type == R_32 ? "R_32" : "R_PC32") <<" " << referencedSymbol << " " << addend;
		}
};

#endif