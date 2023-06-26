.PHONY: emulator
.PHONY: assembler
.PHONY: linker

asssemblerSource := $(wildcard src/assembler_*.cpp)
assemblerHeader := $(wildcard inc/assembler_*.hpp)
emulatorSource := $(wildcard src/emulator_*.cpp)
emulatorHeader := $(wildcard inc/emulator_*.hpp)
linkerSource := $(wildcard src/linker_*.cpp)
linkerHeader := $(wildcard inc/linker_*.hpp)

tmp:
	mkdir -p tmp

tmp/bison.cpp tmp/parser.h: misc/assembler_parser.y | tmp
	cd tmp && bison --warnings=none -o bison.cpp --defines=parser.h ../misc/assembler_parser.y

tmp/lexer.cpp tmp/lexer.h: misc/assembler_lexer.lex | tmp
	cd tmp && flex -o lexer.cpp --header-file=lexer.h ../misc/assembler_lexer.lex

assembler: tmp/bison.cpp tmp/lexer.cpp src/assembler_main.cpp | tmp
	rm -f asembler
	g++ -o asembler $(asssemblerSource) $(assemblerHeader) tmp/*.cpp
	sudo rm -r ./tmp

emulator: src/emulator_main.cpp
	rm -f emulator
	g++ -o emulator $(emulatorSource) $(emulatorHeader)

linker: src/linker_main.cpp
	rm -f linker
	g++ -o linker ${linkerSource} ${linkerHeader}

clean:
	rm -rf tmp
	rm -f *.hex
	rm -f asembler *.o
	rm -f linker *.o
	rm -f emulator *.o

all: assembler
	make clean
	make assembler
	make linker
	make emulator

