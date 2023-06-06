#include "../inc/emulator_timer.hpp"
#include "../inc/emulator_memory.hpp"
#include "../inc/emulator_cpu.hpp"

map<size_t, size_t> Timer::periods = { {0x0, 500}, {0x1, 1000} , {0x2, 1500}, {0x3, 2000},
  									 {0x4, 5000}, {0x5, 10000},{0x6, 30000},{0x7, 60000} 	
  									};

uint64_t Timer::now = 0;
uint64_t Timer::previous = 0;
size_t Timer::period = 500;
bool Timer::started = false;
void Timer::tick(){
	uint64_t time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

	size_t tim_reg = Memory::read4BytesLittleEndian(tim_cfg);
	if(periods.count(tim_reg)){
		period = periods[tim_reg];
	}

	if(started && time - previous >= period && CPU::isInterruptEnabled() && CPU::isTimerInterruptEnabled()){
		CPU::cause = 2;
		CPU::interruptQueue.push_back(CAUSE_TIMER);
		previous = time;
	}
	else{
		started = true;
	}
}

void Timer::initialize(){
	previous = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}