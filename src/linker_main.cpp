#include <iostream>
#include <string>
#include "../inc/linker_exceptions.hpp"
#include "../inc/linker_parser.hpp"
using namespace std;

char *fileOut;

int main(int argc, char** argv) {
		int i=1;
		while(i<argc){
			string arg(argv[i]);
			cout<<arg<<endl;
			if(arg == "-hex"){
				//set hex output
				i++;
			}
			else if(arg.find("-place") != string::npos){
				//place section
				i++;
			}
			else if(arg == "-o"){
				fileOut = argv[i+1];
				i+=2;
			}
			else{
				char* fileIn = argv[i];
				Parser::parseFile(fileIn);
				i++;
			}
		}
	return 0;
}