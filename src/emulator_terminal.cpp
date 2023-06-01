#include "../inc/emulator_terminal.hpp"
#include "../inc/emulator_memory.hpp"
#include "../inc/emulator_cpu.hpp"

termios Terminal::settings;
termios Terminal::previousSettings;

void Terminal::initialize(){
	tcgetattr(fileno(stdin), &previousSettings);
	tcgetattr(fileno(stdin), &settings);

	setvbuf(stdin, NULL, _IONBF, 0);

	if (atexit(Terminal::reset))
        return;

	settings.c_lflag &= ~(IEXTEN | ECHO | ICANON);
	settings.c_cflag &= ~(CSIZE | PARENB);
	settings.c_cflag |= CS8;
	settings.c_cc[VMIN] = 0;
	settings.c_cc[VTIME] = 0;

	tcsetattr(fileno(stdin), TCSANOW, &settings);
}

void Terminal::getChar(){
	char c;
	size_t bytes = read(fileno(stdin), &c, 1);
	if(bytes == 1){
		Memory::write4Bytes(term_in, c);
		CPU::cause = 3;
	}
}

void Terminal::reset(){
	tcsetattr(fileno(stdin), TCSANOW, &previousSettings);
}

void Terminal::putChar(){
	size_t out = Memory::read4Bytes(term_out);
	if(out){
		cout<<char(out & 0xFF);
		cout<<flush;
		Memory::write4Bytes(term_out, 0);
	}
}