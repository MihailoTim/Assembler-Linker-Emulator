
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

class SectionTable{
public:
	struct SectionTableLine{
		string name;
		string content;
		size_t base;
		SectionTableLine(string n, size_t b): name(n), base(b){}
	};

	static size_t totalSize;

	static map<string, SectionTableLine*> sectionTable;

	static map<string, size_t> sectionPlacements;

	static void addNewSectionPlacement(string section, size_t location);

	static void resolveSectionPlacements();

	static void upateSectionVirtualAddresses();

	static void addNewSection(string name, size_t length);

	static void printAllSections();

	static void printSectionHeadersToOutput(ofstream &out);

	static void printSectionContentToOutput(ofstream &out);

	static void checkCollisions(string section, size_t base);

	static bool isOverlapping(size_t base1, size_t size1, size_t base2, size_t size2);
};