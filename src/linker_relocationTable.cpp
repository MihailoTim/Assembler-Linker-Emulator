#include "../inc/linker_relocationTable.hpp"

void RelocationTable::handleReloLine(size_t location, string type, string symbol, size_t addend){
	cout<<location<<" "<<type<<" "<<symbol<<" "<<addend<<endl;
}