cppSource := $(wildcard src/*.cpp)
cSource := $(wildcard src/*.c)
cppHeader := $(wildcard h/*.hpp)
cppHeader := $(wildcard h/*.h)

asembler: src/main.cpp
	g++ -o asembler ${cppSource} ${cSource} ${cppHeader} ${cHeader}


clean: 
	rm -f asembler *.o

all: asembler
	make clean
	make asembler