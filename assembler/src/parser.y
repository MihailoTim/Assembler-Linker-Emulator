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

    int lineCount = 1;
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
%type <symbol> csrreg
%type <symbol> instruction0arg
%type <symbol> instruction1reg
%type <symbol> instruction2reg
%type <symbol> instructionCSRRD
%type <symbol> instructionCSRWR
%type <symbol> instruction2reg1operand
%type <symbol> instructionLD
%type <symbol> instructionST
%type <symbol> instructionJMP
%type <symbol> expression
%type <symbol> operand

%token GLOBAL EXTERN SECTION WORD SKIP EQU END ASCII HALT INT IRET CALL RET JMP BEQ BNE BGT PUSH POP 
%token XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD CSRWR COMMA COLON SEMICOLON PERCENT DOLLAR MID_L_BRACKET MID_R_BRACKET QUOTATION
%token PLUS MINUS

%token <number> REG
%token <symbol> STATUS
%token <symbol> HANDLER
%token <symbol> CAUSE
%token <number> BIN
%token <number> DEC
%token <number> HEX
%token <symbol> SYMBOL
%token <symbol> TEXT

%start program

%%

program:
    line{
        lineCount++;
    } | 
    program line{
        lineCount++;
    }

line:
    label | directive | label directive | instruction | label instruction;

instruction:
    instruction0arg {
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<endl;
    } | 
    instruction1reg REG {
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<endl;
    } | 
    instruction2reg REG COMMA REG {
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<", "<<$4<<endl;
    } | 
    instructionCSRRD csrreg COMMA REG {
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<", "<<$4<<endl;
    } | 
    instructionCSRWR REG COMMA csrreg {
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<", "<<$4<<endl;
    } | 
    instruction2reg1operand REG COMMA REG COMMA operand{
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<", "<<$4<<", "<<$6<<endl;
    } | 
    instructionLD operand COMMA REG{
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<", "<<$4<<endl;
    } |
    instructionST REG COMMA operand{
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<", "<<$4<<endl;
    } |
    instructionJMP operand{
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<$1<<" "<<$2<<endl;
    }

instruction0arg: 
    HALT {
        $$ = "halt";
    } | 
    INT {
        $$ = "int";
    } | 
    IRET {
        $$ = "iret";
    } | 
    RET {
        $$ = "ret";
    };

instruction1reg:
    PUSH {
        $$ = "push";
    } | 
    POP {
        $$ = "pop";
    } | 
    NOT {
        $$ = "not";
    };

instruction2reg:
    XCHG{
        $$ = "xchg";
    } |
    ADD{
        $$ = "add";
    } |  
    SUB{
        $$ = "sub";
    } | 
    MUL{
        $$ = "mul";
    } | 
    DIV{
        $$ = "div";
    } | 
    AND {
        $$ = "and";
    } | 
    OR {
        $$ = "or";
    } | 
    XOR {
        $$ = "xor";
    } | 
    SHR {
        $$ = "shr";
    } | 
    SHL {
        $$ = "shl";
    };

instructionCSRRD:
    CSRRD {
        $$ = "csrrd";
    };

instructionCSRWR:
    CSRWR {
        $$ = "csrwr";
    };

instruction2reg1operand:
    BEQ{
        $$ = "beq";
    } |
    BNE{
        $$ = "bne";
    } | 
    BGT{
        $$ = "bgt";
    };

instructionLD:
    LD{
        $$ = "ld";
    };

instructionST:
    ST{
        $$ = "st";
    }

instructionJMP:
    JMP{
        $$ = "jmp";
    }

operand: 
    DOLLAR literal{
        $$ = "$literal";
    } | 
    DOLLAR symbol{
        $$ = "$symbol";
    } | 
    literal {
        $$ = "literal";   
    } | 
    symbol {
        $$ = "symbol";
    } |
    REG {
        $$ = "%registar";
    } | 
    MID_L_BRACKET REG MID_R_BRACKET {
        $$ = "[%registar]";
    }
        

directive:
    GLOBAL symbolList {
        cout<<lineCount<<": .global ";
        for(string sym : *($2)){
            firstPass.handleGlobalDirective(sym);
            cout<<sym<<" ";
        }
        cout<<endl;

        //firstPass.printSymbolTable();
    } |
    EXTERN symbolList {
        cout<<lineCount<<": .extern ";
        for(string sym : *($2)){
            firstPass.handleExternDirective(sym);
            cout<<sym<<" ";
        }
        cout<<endl;

        //firstPass.printSymbolTable();
    } |
    SECTION symbol {
        cout<<lineCount<<": .section " <<($2)<<endl;

        firstPass.handleSectionDirective(($2));

        //firstPass.printSectionTable();
    } |
    WORD combinedList {
        cout<<lineCount<<": .word ";
        for(string sym : *($2)){
            firstPass.incLocationCounter(4);
            cout<<sym<<" ";
        }
        cout<<endl;

        //cout<<firstPass.getLocationCounter()<<endl;
    } |
    SKIP literalUnsigned {
        firstPass.incLocationCounter($2);
        cout<<lineCount<<": .skip "<<$2<<endl;
    } | 
    ASCII text {
        string txt = *new string($2);
        string str = txt.substr(1, txt.size()-2);
        firstPass.handleAsciiDirective(str);
        firstPass.incLocationCounter(str.size());
        cout<<lineCount<<": .ascii "<<$2<<endl;
    } |
    EQU symbol COMMA expression {
        cout<<lineCount<<": .equ "<<$2<<", "<<$4<<endl;
    } | 
    END {
        cout<<lineCount<<": .end "<<endl;
        firstPass.handleEndDirective();
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
        $$ = $1;
    };

text:
    TEXT {
        $$ = $1;
    }

csrreg:
    STATUS {
        $$ = $1;
    } | 
    HANDLER {
        $$ = $1;
    } | 
    CAUSE {
        $$ = $1;
    };

expression:
    symbol {
        $$ = $1;
    } | 
    literal {
        $$ = strdup(to_string($1).c_str());
    }
%%

void yyerror(char *s) 
{
    cout<<"Syntax error on line: "<<lineCount<<endl;
    throw new Exception("Syntax error");
}