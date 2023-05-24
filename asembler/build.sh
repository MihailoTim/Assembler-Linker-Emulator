mkdir -p tmp
cd tmp
bison -o bison.cpp --defines=parser.h ../src/parser.y
flex -o lexer.cpp --header-file=lexer.h ../src/lexer.lex
cd ..
g++ -o asembler src/*.cpp tmp/*.cpp h/*.hpp
sudo rm -r tmp
