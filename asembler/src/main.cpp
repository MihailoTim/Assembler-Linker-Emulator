#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

extern FILE *yyin;
extern int yyparse();

string outfilename = "";

int main(int argc, char** argv) {
  	if(argc < 2) {
	    printf("Error - Not enough arguments passed\n");
	    exit(-1);
  	}

  	if(argc > 4) {
	    printf("Error - Too many arguments passed\n");
	    exit(-1);
  	}

  	if(argc != 2)
  	{
	    printf("Incorrect number of arguments passed\n");
	    exit(-1);  
  	}


	FILE* file;
	char* filename;

	filename = argv[1];
	file = fopen(filename, "r");
	if(!file) {
		printf("Cannot open the file\n");
		exit(-1);
	}

	yyin = file;
	yyparse();
	fclose(yyin);
	// cout << "kraj" << endl;
	return 0;
}