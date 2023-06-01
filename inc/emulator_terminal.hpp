#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>

using namespace std;

#define term_in 0xFFFFFF04
#define term_out 0xFFFFFF00

class Terminal{
	static termios previousSettings;
	static termios settings;
public:
	static void initialize();

	static void getChar();

	static void putChar();

	static void reset();
};