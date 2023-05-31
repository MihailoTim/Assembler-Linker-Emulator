#include <iostream>
#include <string>

using namespace std;

#ifndef _ARGUMENT_HPP
#define _ARGUMENT_HPP

enum ArgumentType {LITERAL, SYM, REGISTER, REGPLUSSYM, REGPLUSLIT};

enum AddressType {IMMED, MEMDIR, REGDIR, REGIND, REGINDOFF};

class Argument{
public:
	int intVal;
	string stringVal;
	ArgumentType argType;
	AddressType addrType;
	bool hasValue;
	Argument(int intval, string strval,ArgumentType argtype,AddressType addrtype, bool hv) : stringVal(strval), intVal(intval), argType(argtype), addrType(addrtype), hasValue(hv){}

	string toString(){
		string result = "";
		if(addrType == AddressType::IMMED){
			result+="$";
		}
		if(addrType == AddressType::REGIND || addrType == AddressType::REGINDOFF){
			result+="[ ";
		}
		if(argType == ArgumentType::LITERAL){
			result+=to_string(intVal);
		}
		if(argType == ArgumentType::SYM){
			result += stringVal;
		}
		if(argType == ArgumentType::REGISTER || argType == ArgumentType::REGPLUSLIT || argType == ArgumentType::REGPLUSSYM){
			result+="%r"+to_string(intVal);
		}
		if(argType == ArgumentType::REGPLUSLIT || argType == ArgumentType::REGPLUSSYM){
			result+=" + ";
		}
		if(argType == ArgumentType::REGPLUSLIT){
			result+=to_string(intVal);
		}
		if(argType == ArgumentType::REGPLUSSYM){
			result+=stringVal;
		}
		if(addrType == AddressType::REGIND || addrType == AddressType::REGINDOFF){
			result+=" ]";
		}

		return result;
	}

	void printArgument(){
		cout<<toString();
	}
};

#endif