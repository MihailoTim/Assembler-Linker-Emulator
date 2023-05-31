#include <vector>
#include "../inc/assembler_assemblyLine.hpp"

using namespace std;

#ifndef _ASSEMBLY_FILE_HPP
#define _ASSEMBLY_FILE_HPP

class AssemblyFile{
	vector<AssemblyLine*> code;

public:
	inline void writeLine(AssemblyLine *line){code.push_back(line);}

	void printFile(){
		for(AssemblyLine *line : code){
			line->printLine();
		}
	}

	vector<AssemblyLine*> readFile(){return code;}
};

#endif