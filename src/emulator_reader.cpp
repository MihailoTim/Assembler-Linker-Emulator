#include "../inc/emulator_reader.hpp"

vector<string> Reader::readFile(ifstream &in){
	string line;
	vector<string> result;
	string strippedLine;

	while(getline(in, line)){
		strippedLine = line.substr(6);
		size_t pos = strippedLine.find(" ");
		while (pos != std::string::npos) {
			strippedLine.replace(pos, 1, "");
			pos = strippedLine.find(" ", pos);
		}
		strippedLine = strippedLine.substr(0,16);
		result.push_back(strippedLine.substr(0,8));
		result.push_back(strippedLine.substr(8));
	}

	return result;
}