.PHONY: emulator
.PHONY: assembler

asssemblerSource := $(wildcard src/assembler_*.cpp)
assemblerHeader := $(wildcard inc/assembler_*.hpp)
emulatorSource := $(wildcard src/emulator_*.cpp)
emulatorHeader := $(wildcard inc/emulator_*.hpp)

tmp:
	mkdir -p tmp

tmp/bison.cpp tmp/parser.h: misc/assembler_parser.y | tmp
	cd tmp && bison -o bison.cpp --defines=parser.h ../misc/assembler_parser.y

tmp/lexer.cpp tmp/lexer.h: misc/assembler_lexer.lex | tmp
	cd tmp && flex -o lexer.cpp --header-file=lexer.h ../misc/assembler_lexer.lex

assembler: tmp/bison.cpp tmp/lexer.cpp src/assembler_main.cpp | tmp
	rm -f *.hex
	rm -f asembler *.o
	g++ -o asembler $(asssemblerSource) $(assemblerHeader) tmp/*.cpp
	sudo rm -r ./tmp

emulator: src/emulator_main.cpp
	rm -f emulator *.o
	g++ -o emulator $(emulatorSource) $(emulatorHeader)

clean:
	rm -rf tmp
	rm -f *.hex
	rm -f asembler *.o
	rm -f emulator *.o

all: assembler
	make clean
	make assembler
	make emulator

