#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include "../inc/assembler_firstPass.hpp"
#include "../inc/assembler_secondPass.hpp"

using namespace std;

extern FILE *yyin;
extern int yylex();
extern int yyparse();

char* fileIn;
char* fileOut;

void checkArguments(int argc, char **argv){
	if(argc == 1){
		throw new Exception("Insufficient arguments\n");
	}
	else if(argc == 2){
		fileIn = argv[1];
	}
	else if(argc == 3){
		throw new Exception("Can't parse input arguments");
	}
	else if(argc == 4){
		if(strcmp(argv[1], "-o") == 0){
			fileOut = argv[2];
			fileIn = argv[3];
		}
		else if(strcmp(argv[2], "-o") == 0){
			fileOut = argv[3];
			fileIn = argv[2];
		}
		else 
			throw new Exception("Can't parse input arguments");
	}
	else 
		throw new Exception("Can't parse input arguments");
}

int main(int argc, char** argv) {

	checkArguments(argc, argv);

	FirstPass &firstPass = FirstPass::getInstance();
	SecondPass &secondPass = SecondPass::getInstance();

	FILE* file = fopen(fileIn, "r");
	if(file) {
		yyin = file;
		yyparse();
		fclose(file);
		if(firstPass.getStatus() != FirstPass::PassStatus::FINISHED){
			throw new Exception("End of file has been reached but .end was not found");
		}
	}
	else{
		cout<<"Can't open file"<<endl;
	}
	return 0;
}