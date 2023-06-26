#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Printer{
public:
	static void printAllSectionsToHex(const char* fileOut);

	static vector<pair<size_t, string>> sortSectionsByBase();
};