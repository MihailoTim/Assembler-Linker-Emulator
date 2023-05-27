cppSource := $(wildcard src/*.cpp)
cSource := $(wildcard src/*.c)
cppHeader := $(wildcard h/*.hpp)
cppHeader := $(wildcard h/*.h)

asembler: src/main.cpp
	bash build.sh


clean: 
	rm -f asembler *.o

all: asembler
	make clean
	make asembler