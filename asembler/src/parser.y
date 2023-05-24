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

    #define WORD_SIZE 4
    #define INSTRUCTION_SIZE 4

    void yyerror(char *);

    FirstPass& firstPass = FirstPass::getInstance();
%}

%union {
    int number;
    char* symbol;
    std::vector<std::string>* stringVector;
    std::vector<int>* intVector;
}

%type<number> literal
%type<intVector> literalList
%type<symbol> symbol
%type<stringVector> symbolList
%type<symbol> line
%type<stringVector> combinedList

%token GLOBAL EXTERN SECTION WORD SKIP EQU END ASCII HALT INT IRET CALL RET JMP BEQ BNE BGT PUSH POP 
%token XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD CSRWR COMMA COLON SEMICOLON PERCENT DOLLAR MID_L_BRACKET MID_R_BRACKET QUOTATION

%token <number> REG
%token <number> STATUS
%token <number> HANDLER
%token <number> CAUSE
%token <number> NUMBER
%token <symbol> SYMBOL

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

        firstPass.printSymbolTable();
    } |
    EXTERN symbolList {
        cout<<firstPass.getLineCount()<<": .extern ";
        for(string sym : *($2)){
            firstPass.handleExternDirective(sym);
            cout<<sym<<" ";
        }
        cout<<endl;

        firstPass.printSymbolTable();
    } |
    SECTION SYMBOL {
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
    SKIP NUMBER {
        firstPass.incLocationCounter($2);
        //cout<<firstPass.getLocationCounter()<<endl;
    } | 
    END {
        cout<<".end\n";
        //firstPass.printSectionTable();
        firstPass.printSymbolTable();
        YYACCEPT;
    };

combinedList:
    NUMBER {
        $$ = new vector<string>();
        $$->push_back(to_string($1));
    }
    |
    SYMBOL{
        $$ = new vector<string>();
        $$->push_back($1);
    }
    |
    combinedList COMMA NUMBER{
        $$->push_back(to_string($3));
    }
    |
    combinedList COMMA SYMBOL{
        $$->push_back($3);
    };

literalList:
    NUMBER {
        $$ = new vector<int>();
        $$->push_back($1);
    }
    |
    literalList COMMA NUMBER {
        $$->push_back($3)
    };

literal:
    NUMBER { 
        $$ = $1; 
    };

symbolList:
    SYMBOL{
        $$ = new vector<string>();
        $$->push_back($1);
    }
    |
    symbolList COMMA SYMBOL{
        $$->push_back($3);
    }

symbol:
    SYMBOL {
        $$ = $1;
    };

label:
    SYMBOL COLON{
        firstPass.handleLabel($1);
    };
%%

void yyerror(char *s) 
{
    cout<<"Syntax error on line: "<<firstPass.getLineCount()<<endl;
    throw new Exception("Syntax error");
}