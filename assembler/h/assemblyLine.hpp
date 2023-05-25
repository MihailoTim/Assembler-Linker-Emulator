
#include <iostream>
#include <string>
#include <vector>
#include "../h/argument.hpp"

#ifndef _ASSEMBLY_LINE_HPP
#define _ASSEMBLY_LINE_HPP

using namespace std;

class AssemblyLine {
public:
	int lineCount;
	string label;
	string mnemonic;
	vector<Argument*> args;
	string comment;
	AssemblyLine(int lc){
		lineCount = lc;
		label="";
		mnemonic="";
		comment="";
	}

	string toString(){
		string result="";

		result+= to_string(lineCount)+": ";

		if(label.size()){
			result += label + ": ";
		}
		if(mnemonic.size()){
			result += mnemonic + " ";
		}
		for(int i=0;i<args.size();i++){
			if(i!=0){
				result+=", ";
			}
			result += args[i]->toString();
		}

		if(comment.size()){
			result += " "+ comment;
		}
		return result;
	}

	void printLine(){
		cout<<toString()<<(comment.size() > 0 ? "" : "\n");
	}
};

#endif