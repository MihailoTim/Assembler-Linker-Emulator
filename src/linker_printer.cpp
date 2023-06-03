#include "../inc/linker_printer.hpp"
#include "../inc/linker_sectionTable.hpp"
#include <fstream>
#include <algorithm>

void Printer::printAllSectionsToHex(){
	map<size_t, string> outputMap;
	ofstream out("output.hex");
	for(auto it = SectionTable::sectionTable.begin(); it!= SectionTable::sectionTable.end(); it++){
		cout<<it->first<<" "<<it->second->content.size()<<endl;
		string res = "";
		size_t location = it->second->base;
		cout<<location<<endl;
		cout<<it->second->content.size()<<endl;
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
		if(it->first == bufferedAddress + 4){
			out<<setw(8)<<setfill('0')<<hex<<bufferedAddress<<": "<<buffered<<" "<<it->second<<endl;
			bufferedAddress = -1;
		}
		else{
			buffered = it->second;
			bufferedAddress = it->first;
		}
	}
	if(bufferedAddress > 0){
		out<<setw(8)<<setfill('0')<<hex<<bufferedAddress<<buffered<<" 00 00 00 00"<<endl;
	}
}