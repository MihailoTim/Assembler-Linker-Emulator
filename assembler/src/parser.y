%{
    #include <vector>
    #include <iostream>
    #include <map>
    #include <string.h>
    #include <stdlib.h>
    #include "../h/exceptions.hpp"
    #include "../h/firstPass.hpp"
    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;

    using namespace std;

    void yyerror(char *);

    FirstPass& firstPass = FirstPass::getInstance();
%}

%union {
    int number;
    char* symbol;
    std::vector<std::string>* stringVector;
    std::vector<int>* intVector;
}

%type <number> literal
%type <number> literalSigned
%type <number> literalUnsigned
%type <symbol> label
%type <intVector> literalList
%type <symbol> symbol
%type <stringVector> symbolList
%type <symbol> line
%type <stringVector> combinedList
%type <symbol> text

%token GLOBAL EXTERN SECTION WORD SKIP EQU END ASCII HALT INT IRET CALL RET JMP BEQ BNE BGT PUSH POP 
%token XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD CSRWR COMMA COLON SEMICOLON PERCENT DOLLAR MID_L_BRACKET MID_R_BRACKET QUOTATION
%token PLUS MINUS

%token <number> REG
%token <number> STATUS
%token <number> HANDLER
%token <number> CAUSE
%token <number> BIN
%token <number> DEC
%token <number> HEX
%token <symbol> SYMBOL
%token <symbol> TEXT

%start program

%%

program:
    line{
        int lineCount = firstPass.incAndGetLineCount();
    } | 
    program line{
        int lineCount = firstPass.incAndGetLineCount();
    }

line:
    label | directive | label directive;

directive:
    GLOBAL symbolList {
        cout<<firstPass.getLineCount()<<": .global ";
        for(string sym : *($2)){
            firstPass.handleGlobalDirective(sym);
            cout<<sym<<" ";
        }
        cout<<endl;

        //firstPass.printSymbolTable();
    } |
    EXTERN symbolList {
        cout<<firstPass.getLineCount()<<": .extern ";
        for(string sym : *($2)){
            firstPass.handleExternDirective(sym);
            cout<<sym<<" ";
        }
        cout<<endl;

        //firstPass.printSymbolTable();
    } |
    SECTION symbol {
        cout<<firstPass.getLineCount()<<": .section " <<($2)<<endl;

        firstPass.handleSectionDirective(($2));

        //firstPass.printSectionTable();
    } |
    WORD combinedList {
        cout<<firstPass.getLineCount()<<": .word ";
        for(string sym : *($2)){
            firstPass.incLocationCounter(4);
            cout<<sym<<" ";
        }
        cout<<endl;

        //cout<<firstPass.getLocationCounter()<<endl;
    } |
    SKIP literalUnsigned {
        firstPass.incLocationCounter($2);
        cout<<firstPass.getLineCount()<<": .skip "<<$2<<endl;
    } | 
    ASCII text {
        string txt = *new string($2);
        string str = txt.substr(1, txt.size()-2);
        firstPass.handleAsciiDirective(str);
        firstPass.incLocationCounter(str.size());
        cout<<firstPass.getLineCount()<<": .ascii "<<$2<<endl;
    } |
    END {
        cout<<".end\n";
        //firstPass.printSectionTable();
        firstPass.printSymbolTable();
        firstPass.printSectionTable();
        YYACCEPT;
    };

combinedList:
    literal {
        $$ = new vector<string>();
        $$->push_back(to_string($1));
    }
    |
    symbol {
        $$ = new vector<string>();
        $$->push_back($1);
    }
    |
    combinedList COMMA literal {
        $$->push_back(to_string($3));
    }
    |
    combinedList COMMA symbol{
        $$->push_back($3);
    };

literalList:
    literal {
        $$ = new vector<int>();
        $$->push_back($1);
    }
    |
    literalList COMMA literal {
        $$->push_back($3)
    };

literal:
    literalUnsigned {
        $$ = $1;
    } |
    literalSigned {
        $$ = $1;
    };

literalSigned: 
    PLUS literalUnsigned {
        $$ = $2;
    } |
    MINUS literalUnsigned {
        $$ = -$2;
    };

literalUnsigned:
    BIN {
        $$ = $1;
    } | 
    DEC {
        $$ = $1;
    } | 
    HEX {
        $$ = $1;
    };


symbolList:
    symbol {
        $$ = new vector<string>();
        $$->push_back($1);
    } |
    symbolList COMMA symbol{
        $$->push_back($3);
    }

symbol:
    SYMBOL {
        $$ = $1;
    };

label:
    symbol COLON {
        firstPass.handleLabel($1);
        cout<<($1)<<endl;
        $$ = $1;
    };

text:
    TEXT {
        $$ = $1;
    }
%%

void yyerror(char *s) 
{
    cout<<"Syntax error on line: "<<firstPass.getLineCount()<<endl;
    throw new Exception("Syntax error");
}