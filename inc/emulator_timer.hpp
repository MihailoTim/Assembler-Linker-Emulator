#include <iostream>
#include <map>
#include <chrono>
using namespace std;

#define tim_cfg 0xFFFFFF10

class Timer{
public:
	static map<size_t, size_t> periods;

	static void tick();
	static void initialize();
	static uint64_t now;
	static uint64_t previous;
	static size_t period;

	static bool started;
};