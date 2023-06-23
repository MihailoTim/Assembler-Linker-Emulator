%{
    #include <vector>
    #include <iostream>
    #include <map>
    #include <string.h>
    #include <stdlib.h>
    #include "../inc/assembler_exceptions.hpp"
    #include "../inc/assembler_firstPass.hpp"
    #include "../inc/assembler_assemblyLine.hpp"
    #include "../inc/assembler_assemblyFile.hpp"
    #include "../inc/assembler_secondPass.hpp"
    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;

    using namespace std;

    void yyerror(const char *);

    FirstPass& firstPass = FirstPass::getInstance();

    int lineCount = 1;

    bool isEnd = false;

    AssemblyFile *file = new AssemblyFile();

    AssemblyLine *currentLine = new AssemblyLine(lineCount);

    Argument* delayedOperand;
%}

%union {
    int intVal;
    std::string *stringVal;
    std::vector<std::string>* intVector;
    std::vector<std::string>* stringVector;
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
%type <intVal> csrreg
%type <stringVal> instruction0arg
%type <stringVal> instruction1reg
%type <stringVal> instruction2reg
%type <stringVal> instructionCSRRD
%type <stringVal> instructionCSRWR
%type <stringVal> instruction2reg1operand
%type <stringVal> instructionLD
%type <stringVal> instructionST
%type <stringVal> instructionJMP
%type <stringVal> instructionCALL
%type <stringVector> expression
%type <stringVal> operand
%type <stringVal> branchOperand
%type <stringVal> comment

%token GLOBAL EXTERN SECTION WORD SKIP EQU END ASCII HALT INT IRET CALL RET JMP BEQ BNE BGT PUSH POP 
%token XCHG ADD SUB MUL DIV NOT AND OR XOR SHL SHR LD ST CSRRD CSRWR COMMA COLON SEMICOLON PERCENT DOLLAR MID_L_BRACKET MID_R_BRACKET QUOTATION
%token SMALL_L_BRACKET SMALL_R_BRACKET NEWLINE
%token PLUS MINUS
%token FEND 0 

%token <intVal> REG
%token <intVal> STATUS
%token <intVal> HANDLER
%token <intVal> CAUSE
%token <intVal> BIN
%token <intVal> DEC
%token <intVal> HEX
%token <stringVal> SYMBOL
%token <stringVal> TEXT
%token <stringVal> COMMENT

%define parse.error verbose;

%start program
%%

program:
    program line{
        lineCount++;
        file->writeLine(currentLine);
        currentLine = new AssemblyLine(lineCount);
        if(isEnd){
            //file->printFile();
            firstPass.setStatus(FirstPass::PassStatus::FINISHED);
            SecondPass &secondPass = SecondPass::getInstance();
            secondPass.setFile(file);
            secondPass.start();
            YYACCEPT;
        }
    } | 
    line{
        lineCount++;
        file->writeLine(currentLine);
        currentLine = new AssemblyLine(lineCount);
        if(isEnd){
            //file->printFile();
            firstPass.setStatus(FirstPass::PassStatus::FINISHED);
            SecondPass &secondPass = SecondPass::getInstance();
            secondPass.setFile(file);
            secondPass.start();
            YYACCEPT;
        }
    }

line:
    label NEWLINE| label directive NEWLINE| label directive comment |
    label NEWLINE| label instruction NEWLINE| label instruction comment |
    directive NEWLINE| directive comment |
    instruction NEWLINE| instruction comment | 
    comment | lastLine | NEWLINE

instruction:
    instruction0arg {
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } | 
    instruction1reg REG {
        currentLine->args.push_back(new Argument($2, to_string($2), ArgumentType::REGISTER, AddressType::REGDIR, false));
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } | 
    instruction2reg REG COMMA REG {
        currentLine->args.push_back(new Argument($2, to_string($2), ArgumentType::REGISTER, AddressType::REGDIR, false));
        currentLine->args.push_back(new Argument($4, to_string($4), ArgumentType::REGISTER, AddressType::REGDIR, false));
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } | 
    instructionCSRRD csrreg COMMA REG {
        currentLine->args.push_back(new Argument($2, to_string($2), ArgumentType::REGISTER, AddressType::REGDIR, false));
        currentLine->args.push_back(new Argument($4, to_string($4), ArgumentType::REGISTER, AddressType::REGDIR, false));
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } | 
    instructionCSRWR REG COMMA csrreg {
        currentLine->args.push_back(new Argument($2, to_string($2), ArgumentType::REGISTER, AddressType::REGDIR, false));
        currentLine->args.push_back(new Argument($4, to_string($4), ArgumentType::REGISTER, AddressType::REGDIR, false));
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } | 
    instruction2reg1operand REG COMMA REG COMMA branchOperand{
        currentLine->args.push_back(new Argument($2, to_string($2), ArgumentType::REGISTER, AddressType::REGDIR, false));
        currentLine->args.push_back(new Argument($4, to_string($4), ArgumentType::REGISTER, AddressType::REGDIR, false));
        currentLine->args.push_back(delayedOperand);
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } | 
    instructionLD operand COMMA REG{
        currentLine->args.push_back(delayedOperand);
        currentLine->args.push_back(new Argument($4, to_string($4), ArgumentType::REGISTER, AddressType::REGDIR, false));
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } |
    instructionST REG COMMA operand{
        currentLine->args.push_back(new Argument($2, to_string($2), ArgumentType::REGISTER, AddressType::REGDIR, false));
        currentLine->args.push_back(delayedOperand);
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } |
    instructionJMP branchOperand{
        currentLine->args.push_back(delayedOperand);
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
    } | 
    instructionCALL branchOperand{
        currentLine->args.push_back(delayedOperand);
        firstPass.incLocationCounter(4);
        currentLine->mnemonic = *($1);
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

instructionCALL:
    CALL{
        $$ = new string("call");
    }

branchOperand:
    literal {
        delayedOperand = new Argument($1, to_string($1), ArgumentType::LITERAL, AddressType::MEMDIR, true);
        $$ = new string(to_string($1));   
    } | 
    symbol {
        delayedOperand = new Argument(0, *$1, ArgumentType::SYM, AddressType::MEMDIR, false);
        firstPass.handleSymbolReference(*$1);
        $$ = new string(*($1));
    }

operand: 
    DOLLAR literal{
        delayedOperand = new Argument($2, to_string($2), ArgumentType::LITERAL, AddressType::IMMED, true);
        $$ = new string("$"+ to_string($2));
    } | 
    DOLLAR symbol{
        delayedOperand = new Argument(0, *$2, ArgumentType::SYM, AddressType::IMMED, false);
        firstPass.handleSymbolReference(*$2);
        $$ = new string("$" + *($2));
    } | 
    literal {
        delayedOperand = new Argument($1, to_string($1), ArgumentType::LITERAL, AddressType::MEMDIR, true);
        $$ = new string(to_string($1));   
    } | 
    symbol {
        delayedOperand = new Argument(0, *$1, ArgumentType::SYM, AddressType::MEMDIR, false);
        firstPass.handleSymbolReference(*$1);
        $$ = new string(*($1));
    } |
    REG {
        delayedOperand = new Argument($1, to_string($1), ArgumentType::REGISTER, AddressType::REGDIR, false);
        $$ = new string("%r"+to_string($1));
    } | 
    MID_L_BRACKET REG MID_R_BRACKET {
        delayedOperand = new Argument($2, to_string($2), ArgumentType::REGISTER, AddressType::REGIND, false);
        $$ = new string("[ %r" + to_string($2) + "]");
    } |
    MID_L_BRACKET REG PLUS literal MID_R_BRACKET {
        delayedOperand = new Argument($2, to_string($4), ArgumentType::REGPLUSLIT, AddressType::REGINDOFF, false);
        $$ = new string("[ %r" + to_string($2) + " + " + to_string($4) + " ]");
    } |
    MID_L_BRACKET REG PLUS symbol MID_R_BRACKET {
        delayedOperand = new Argument($2, *$4, ArgumentType::REGPLUSSYM, AddressType::REGINDOFF, false);
        firstPass.handleSymbolReference(*$4);
        $$ = new string("[ %r" + to_string($2) + " + " + *($4) + " ]");
    };

directive:
    GLOBAL symbolList {
        currentLine->mnemonic = ".global";
        for(string sym : *($2)){
            currentLine->args.push_back(new Argument(0, sym, ArgumentType::SYM, AddressType::MEMDIR, false));
            firstPass.handleGlobalDirective(sym);
        }
    } |
    EXTERN symbolList {
        currentLine->mnemonic = ".extern";
        for(string sym : *($2)){
            currentLine->args.push_back(new Argument(0, sym, ArgumentType::SYM, AddressType::MEMDIR, false));
            firstPass.handleExternDirective(sym);
        }
    } |
    SECTION symbol {
        currentLine->mnemonic = ".section";
        currentLine->args.push_back(new Argument(0, *$2, ArgumentType::SYM, AddressType::MEMDIR, false));
        firstPass.handleSectionDirective(*($2));
    } |
    WORD combinedList {
        currentLine->mnemonic = ".word";
        for(string sym : *($2)){
            firstPass.incLocationCounter(4);
        }
    } |
    SKIP literalUnsigned {
        currentLine->mnemonic = ".skip";
        currentLine->args.push_back(new Argument($2, to_string($2), ArgumentType::LITERAL, AddressType::MEMDIR, false));
        firstPass.incLocationCounter($2);
    } | 
    ASCII text {
        currentLine->mnemonic = ".ascii";
        string txt = *new string(*$2);
        string str = txt.substr(1, txt.size()-2);
        firstPass.handleAsciiDirective(str);
        currentLine->args.push_back(new Argument(0, str, ArgumentType::SYM, AddressType::MEMDIR, false));
        firstPass.incLocationCounter(str.size());
    } |
    EQU symbol COMMA expression {
        currentLine->mnemonic = ".equ";

        currentLine->args.push_back(new Argument(0, *$2, ArgumentType::SYM, AddressType::MEMDIR, false));

        firstPass.handleEquDirective(*$2, *$4);
    } | 
    END {
        currentLine->mnemonic = ".end";
        firstPass.handleEndDirective();
        isEnd = true;
    };

combinedList:
    literal {
        $$ = new vector<string>();
        currentLine->args.push_back(new Argument($1, to_string($1), ArgumentType::LITERAL, AddressType::MEMDIR, false));
        $$->push_back(to_string($1));
    }
    |
    symbol {
        $$ = new vector<string>();
        currentLine->args.push_back(new Argument(0, *$1, ArgumentType::SYM, AddressType::MEMDIR, false));
        firstPass.handleSymbolReference(*$1);
        $$->push_back(*$1);
    }
    |
    combinedList COMMA literal {
        currentLine->args.push_back(new Argument($3, to_string($3), ArgumentType::LITERAL, AddressType::MEMDIR, false));
        $$->push_back(to_string($3));
    }
    |
    combinedList COMMA symbol{
        currentLine->args.push_back(new Argument(0, *$3, ArgumentType::SYM, AddressType::MEMDIR, false));
        firstPass.handleSymbolReference(*$3);
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
        firstPass.handleLabel(*$1);
        currentLine->label = *$1;
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
        $$ = new vector<string>();
        currentLine->args.push_back(new Argument(0, *$1, ArgumentType::SYM, AddressType::MEMDIR, false));
        $$->push_back(*$1);
    } | 
    literal {
        $$ = new vector<string>();
        currentLine->args.push_back(new Argument($1, to_string($1), ArgumentType::LITERAL, AddressType::MEMDIR, false));
        $$->push_back(to_string($1));
    } | 
    expression PLUS symbol{
        currentLine->args.push_back(new Argument(0, *$3, ArgumentType::SYM, AddressType::MEMDIR, false));
        $$->push_back("+");
        $$->push_back(*$3);
    } | 
    expression PLUS literal{
        currentLine->args.push_back(new Argument($3, to_string($3), ArgumentType::LITERAL, AddressType::MEMDIR, false));
        $$->push_back("+");
        $$->push_back(to_string($3));
    } |
    expression MINUS symbol{
        currentLine->args.push_back(new Argument(0, *$3, ArgumentType::SYM, AddressType::MEMDIR, false));
        $$->push_back("-");
        $$->push_back(*$3);
    } |
    expression MINUS literal{
        currentLine->args.push_back(new Argument($3, to_string($3), ArgumentType::LITERAL, AddressType::MEMDIR, false));
        $$->push_back("-");
        $$->push_back(to_string($3));
    }

comment:
    COMMENT{
        currentLine->comment = *$1;
    };

lastLine:
    END FEND{
        currentLine->mnemonic = ".end";
        firstPass.handleEndDirective();
        isEnd = true;
    }
%%

void yyerror(const char *s) 
{
    cout<<"syntax error on line: "<<lineCount<<endl;
    cout<<s<<endl;
    throw new Exception("Syntax error");
}