#include "../inc/linker_printer.hpp"
#include "../inc/linker_sectionTable.hpp"
#include <fstream>
#include <algorithm>

void Printer::printAllSectionsToHex(){
	map<size_t, string> outputMap;
	ofstream out("output.hex");
	cout<<"STARTED PRINTING\n";
	for(auto it = SectionTable::sectionTable.begin(); it!= SectionTable::sectionTable.end(); it++){
		string res = "";
		size_t location = it->second->base;
		int i=0;
		for(i = 0; i < (8-it->second->content.size()%8)%8;i++)
			it->second->content+="00";
		while(i<it->second->content.size()/8){
			string line = it->second->content.substr(i*8, 8);
			i++;
			string tmp = "";
			for(int j=0;j<=3;j++){
				tmp += line.substr(j*2, 2) + (j==3 ? "" : " ");
			}
			outputMap.insert(make_pair(location, tmp));
			location+=4;
		}
		// for(int i=0;i<it->second->content.size();i++){
		// 	res += it->second->content[i];
		// 	if(i%2 == 0){
		// 		res+=" ";
		// 	}
		// 	if(i%8 == 0){
		// 		outputMap.insert(make_pair(location, res));
		// 		location += 4;
		// 		res = "";
		// 	}
		// }
		// if(res.size()){
		// 	for(int j=(8-res.size())/2;j>=0;j--){
		// 		res+= "00";
		// 		if(j > 0)
		// 			res+=" ";
		// 	}
		// 	outputMap.insert(make_pair(location, res));
		// 	location += 4;
		// }
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