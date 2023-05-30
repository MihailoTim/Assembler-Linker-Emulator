#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Reader{
public:
	static vector<string> readFile(ifstream&);
};