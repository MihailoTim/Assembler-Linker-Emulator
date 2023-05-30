cppSource := $(wildcard src/*.cpp)
cSource := $(wildcard src/*.c)
cppHeader := $(wildcard inc/*.hpp)
cppHeader := $(wildcard inc/*.h)

emulator: src/main.cpp
	g++ -o emulator ${cppSource} ${cSource} ${cppHeader} ${cHeader}

clean: 
	rm -f emulator *.o

all: emulator
	make clean
	make emulator