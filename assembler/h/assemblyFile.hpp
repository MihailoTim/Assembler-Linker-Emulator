
#include <vector>
#include "../h/assemblyLine.hpp"

using namespace std;

class AssemblyFile{
	vector<AssemblyLine*> code;

public:
	inline void writeLine(AssemblyLine *line){code.push_back(line);}

	void printFile(){
		for(AssemblyLine *line : code){
			line->printLine();
		}
	}
};