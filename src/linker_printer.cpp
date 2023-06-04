#include "../inc/linker_printer.hpp"
#include "../inc/linker_sectionTable.hpp"
#include <fstream>
#include <algorithm>

void Printer::printAllSectionsToHex(){
	map<size_t, string> outputMap;
	ofstream out("output.hex");
	for(auto it = SectionTable::sectionTable.begin(); it!= SectionTable::sectionTable.end(); it++){
		string res = "";
		size_t location = it->second->base;
		for(int i=0;i<it->second->content.size();i++){
			res += it->second->content[i];
			if(res.size() == 8){
				string tmp = "";
				for(int j=3;j>=0;j--){
					tmp += res.substr(j*2, 2) + (j == 0 ? "" : " ");
				}
				outputMap.insert(make_pair(location, tmp));
				location += 4;
				res = "";
			}
		}
		if(res.size()){
			string tmp = "";
			for(int j=3;j>=0;j--){
				tmp += res.substr(j*2, 2) + (j == 0 ? "" : " ");
			}
			outputMap.insert(make_pair(location, tmp));
			location += 4;
		}
	}
	string buffered = "";
	int bufferedAddress = -1;
	for(auto it = outputMap.begin(); it!=outputMap.end(); it++){
		if(it->first % 8 == 0){
			if(outputMap.count(it->first+4)){
				buffered = it->second;
				bufferedAddress = it->first;
			}
			else{
				out<<setw(8)<<setfill('0')<<hex<<it->first<<": "<<it->second<<" "<<"00 00 00 00"<<endl;	
				buffered = "";
				bufferedAddress = -1;
			}
		}
		else if(it->first% 8 == 4){
			if(buffered.size()){
				out<<setw(8)<<setfill('0')<<hex<<it->first-4<<": "<<buffered<<" "<<it->second<<endl;
				bufferedAddress = -1;	
				buffered = "";
			}
			else{
				out<<setw(8)<<setfill('0')<<hex<<it->first - 4<<": "<<"00 00 00 00"<<" "<<it->second<<endl;
				bufferedAddress = -1;	
				buffered = "";
			}
		}
	}
}