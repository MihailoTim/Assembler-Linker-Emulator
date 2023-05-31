asssemblerSource := $(wildcard src/assembler_*.cpp)
assemblerHeader := $(wildcard inc/assembler_*.hpp)
emulatorSource := $(wildcard src/emulator_*.cpp)
emulatorHeader := $(wildcard inc/emulator_*.hpp)

tmp:
	mkdir -p tmp

tmp/bison.cpp tmp/parser.h: misc/parser_assembler.y | tmp
	cd tmp && bison -o bison.cpp --defines=parser.h ../misc/parser_assembler.y

tmp/lexer.cpp tmp/lexer.h: misc/lexer_assembler.lex | tmp
	cd tmp && flex -o lexer.cpp --header-file=lexer.h ../misc/lexer_assembler.lex

assembler: tmp/bison.cpp tmp/lexer.cpp src/assembler_main.cpp | tmp
	g++ -o asembler $(asssemblerSource) $(assemblerHeader) tmp/*.cpp
	sudo rm -r ./tmp

emulator: src/emulator_main.cpp
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

