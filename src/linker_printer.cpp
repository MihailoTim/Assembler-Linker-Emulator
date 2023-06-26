#include "../inc/linker_printer.hpp"
#include "../inc/linker_sectionTable.hpp"
#include <fstream>
#include <algorithm>

void Printer::printAllSectionsToHex(const char* fileOut){
	map<size_t, string> outputMap;
	ofstream out(fileOut);
	for(auto it = SectionTable::sectionTable.begin(); it!= SectionTable::sectionTable.end(); it++){
		string res = "";
		size_t location = it->second->base;
		int i=0;
		while(i<it->second->content.size()/2){
			string line = it->second->content.substr(i*2, 2);
			i++;
			outputMap.insert(make_pair(location, line));
			location++;
		}
	}
	size_t line = (outputMap.begin()->first / 8) * 8;
	vector<string> bytes(8,"00");
	for(auto it = outputMap.begin(); it!=outputMap.end(); it++){
		size_t newLine = (it->first/8) * 8;
		if(line != newLine){
			out<<setw(8)<<setfill('0')<<hex<<line<<": ";
			for(int i=0;i<8;i++){
				out<<bytes[i]<< (i==7 ? "" : " ");
			}
			out<<endl;
			line = newLine;
			bytes = vector<string>(8,"00");
		}
		bytes[it->first % 8] = it->second;
	}
	out<<setw(8)<<setfill('0')<<hex<<line<<": ";
	for(int i=0;i<8;i++){
		out<<bytes[i]<< (i==7 ? "" : " ");
	}
	out<<endl;
}