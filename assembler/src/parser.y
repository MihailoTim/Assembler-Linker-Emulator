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

    string currentLine = "";
%}

%union {
    int intVal;
    std::string *stringVal;
    std::vector<std::string>* stringVector;
    std::vector<int>* intVector;
}

%type <intVal> literal
%type <intVal> literalSigned
%type <intVal> literalUnsigned
%type <stringVal> label
%type <intVector> literalList
%type <stringVal> symbol
%type <stringVector> symbolList
%type <stringVal> line
%type <stringVector> combinedList
%type <stringVal> text
%type <stringVal> csrreg
%type <stringVal> instruction0arg
%type <stringVal> instruction1reg
%type <stringVal> instruction2reg
%type <stringVal> instructionCSRRD
%type <stringVal> instructionCSRWR
%type <stringVal> instruction2reg1operand
%type <stringVal> instructionLD
%type <stringVal> instructionST
%type <stringVal> instructionJMP
%type <stringVal> expression
%type <stringVal> operand
%type <stringVal> comment

%token GLOBAL EXTERN SECTION WORD SKIP EQU END ASCII HALT INT IRET CALL RET JMP BEQ BNE BGT PUSH POP 
%token XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD CSRWR COMMA COLON SEMICOLON PERCENT DOLLAR MID_L_BRACKET MID_R_BRACKET QUOTATION
%token SMALL_L_BRACKET SMALL_R_BRACKET
%token PLUS MINUS

%token <intVal> REG
%token <stringVal> STATUS
%token <stringVal> HANDLER
%token <stringVal> CAUSE
%token <intVal> BIN
%token <intVal> DEC
%token <intVal> HEX
%token <stringVal> SYMBOL
%token <stringVal> TEXT
%token <stringVal> COMMENT

%start program

%%

program:
    line{
        lineCount++;
        currentLine = "";
    } | 
    program line{
        lineCount++;
        currentLine = "";
    }

line:
    label | label directive | label directive comment |
    label | label instruction | label instruction comment |
    directive | directive comment |
    instruction | instruction comment | 
    comment

instruction:
    instruction0arg {
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<endl;
    } | 
    instruction1reg REG {
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<$2<<endl;
    } | 
    instruction2reg REG COMMA REG {
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<$2<<", "<<$4<<endl;
    } | 
    instructionCSRRD csrreg COMMA REG {
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<*($2)<<", "<<$4<<endl;
    } | 
    instructionCSRWR REG COMMA csrreg {
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<$2<<", "<<*($4)<<endl;
    } | 
    instruction2reg1operand REG COMMA REG COMMA operand{
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<$2<<", "<<$4<<" "<<*($6)<<endl;
    } | 
    instructionLD operand COMMA REG{
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<*($2)<<", "<<$4<<endl;
    } |
    instructionST REG COMMA operand{
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<$2<<", "<<*($4)<<endl;
    } |
    instructionJMP operand{
        currentLine+= (*$1);
        firstPass.incLocationCounter(4);
        cout<<lineCount<<": "<<*($1)<<" "<<*($2)<<endl;
    }

instruction0arg: 
    HALT {
        $$ = new string("halt");
    } | 
    INT {
        $$ = new string("int");
    } | 
    IRET {
        $$ = new string("iret");
    } | 
    RET {
        $$ = new string("ret");
    };

instruction1reg:
    PUSH {
        $$ = new string("push");
    } | 
    POP {
        $$ = new string("pop");
    } | 
    NOT {
        $$ = new string("not");
    };

instruction2reg:
    XCHG{
        $$ = new string("xchg");
    } |
    ADD{
        $$ = new string("add");
    } |  
    SUB{
        $$ = new string("sub");
    } | 
    MUL{
        $$ = new string("mul");
    } | 
    DIV{
        $$ = new string("div");
    } | 
    AND {
        $$ = new string("and");
    } | 
    OR {
        $$ = new string("or");
    } | 
    XOR {
        $$ = new string("xor");
    } | 
    SHR {
        $$ = new string("shr");
    } | 
    SHL {
        $$ = new string("shl");
    };

instructionCSRRD:
    CSRRD {
        $$ = new string("csrrd");
    };

instructionCSRWR:
    CSRWR {
        $$ = new string("csrwr");
    };

instruction2reg1operand:
    BEQ{
        $$ = new string("beq");
    } |
    BNE{
        $$ = new string("bne");
    } | 
    BGT{
        $$ = new string("bgt");
    };

instructionLD:
    LD{
        $$ = new string("ld");
    };

instructionST:
    ST{
        $$ = new string("st");
    }

instructionJMP:
    JMP{
        $$ = new string("jmp");
    }

operand: 
    DOLLAR literal{
        $$ = new string("$"+ to_string($2));
    } | 
    DOLLAR symbol{
        $$ = new string("$" + *($2));
    } | 
    literal {
        $$ = new string(to_string($1));   
    } | 
    symbol {
        $$ = new string(*($1));
    } |
    REG {
        $$ = new string("%r"+to_string($1));
    } | 
    MID_L_BRACKET REG MID_R_BRACKET {
        $$ = new string("[ %r" + to_string($2) + "]");
    } |
    MID_L_BRACKET REG PLUS literal MID_R_BRACKET {
        $$ = new string("[ %r" + to_string($2) + " + " + to_string($4) + " ]");
    } |
    MID_L_BRACKET REG PLUS symbol MID_R_BRACKET {
        $$ = new string("[ %r" + to_string($2) + " + " + *($4) + " ]");
    };

directive:
    GLOBAL symbolList {
        currentLine+= ".global ";
        cout<<lineCount<<": .global ";
        for(string sym : *($2)){
            firstPass.handleGlobalDirective(sym);
            cout<<sym<<" ";
        }
        cout<<endl;
    } |
    EXTERN symbolList {
        currentLine+= ".extern ";
        cout<<lineCount<<": .extern ";
        for(string sym : *($2)){
            firstPass.handleExternDirective(sym);
            cout<<sym<<" ";
        }
        cout<<endl;
    } |
    SECTION symbol {
        currentLine+= ".section ";
        cout<<lineCount<<": .section " <<*($2)<<endl;

        firstPass.handleSectionDirective(*($2));
    } |
    WORD combinedList {
        currentLine+= ".word ";
        cout<<lineCount<<": .word ";
        for(string sym : *($2)){
            firstPass.incLocationCounter(4);
            cout<<sym<<" ";
        }
        cout<<endl;
    } |
    SKIP literalUnsigned {
        currentLine+= ".skip ";
        firstPass.incLocationCounter($2);
        cout<<lineCount<<": .skip "<<$2<<endl;
    } | 
    ASCII text {
        currentLine+= ".ascii ";
        string txt = *new string(*$2);
        string str = txt.substr(1, txt.size()-2);
        firstPass.handleAsciiDirective(str);
        firstPass.incLocationCounter(str.size());
        cout<<lineCount<<": .ascii "<<$2<<endl;
    } |
    EQU symbol COMMA expression {
        currentLine+= ".equ ";
        cout<<lineCount<<": .equ "<<*($2)<<", "<<*($4)<<endl;
    } | 
    END {
        currentLine+= ".end ";
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
        $$->push_back(*$1);
    }
    |
    combinedList COMMA literal {
        $$->push_back(to_string($3));
    }
    |
    combinedList COMMA symbol{
        $$->push_back(*$3);
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
        $$->push_back(*$1);
    } |
    symbolList COMMA symbol{
        $$->push_back(*$3);
    }

symbol:
    SYMBOL {
        $$ = $1;
    };

label:
    symbol COLON {
        currentLine+= (*$1) + ": ";
        firstPass.handleLabel(*$1);
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
        $$ = new string(to_string($1));
    }

comment:
    COMMENT{
        currentLine += *$1;
        cout<<*($1);
    };
%%

void yyerror(char *s) 
{
    cout<<"Syntax error on line: "<<lineCount<<endl;
    throw new Exception("Syntax error");
}