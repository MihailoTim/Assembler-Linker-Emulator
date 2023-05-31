%{
    #include <iostream>
    #include <vector>
    #include "../tmp/parser.h"
    using namespace std;
    extern int yylex();

    //nounput noinput batch debug
%}

%option noyywrap

letter [A-Za-z]
symbol [_A-Za-z][_A-Za-z0-9]*
dec 0|([1-9][0-9]*)
hex 0[xX][A-Fa-f0-9]+
oct 0([1-9][0-9]*)
bin ([01]+)b
blank [ \t\n]
comment #.*\n
text \"([^\"]*)\"
pc %pc
sp %sp
cause %cause
handler %handler
status %status
regX (%r)([0-9])
reg1X (%r1)([0-5])

%%
".global" return GLOBAL; 
".extern" return EXTERN;
".section" return SECTION;
".word" return WORD;
".skip" return SKIP;
".ascii" return ASCII;
".equ" return EQU;
".end" return END;
"halt" return HALT; 
"int" return INT;
"iret" return IRET;
"call" return CALL;
"ret" return RET; 
"jmp" return JMP;
"beq" return BEQ;
"bne" return BNE;
"bgt" return BGT;
"push" return PUSH;
"pop" return POP;
"xchg" return XCHG;
"add" return ADD;
"sub" return SUB;
"mul" return MUL;
"div" return DIV;
"not" return NOT;
"and" return AND;
"or" return OR;
"xor" return XOR;
"shl" return SHL;
"shr" return SHR;
"ld" return LD;
"st" return ST;
"csrrd" return CSRRD;
"csrwr" return CSRWR;
"," return COMMA;
":" return COLON;
";" return SEMICOLON;
"%" return PERCENT;
"$" return DOLLAR;
"[" return MID_L_BRACKET;
"]" return MID_R_BRACKET;
"(" return SMALL_L_BRACKET;
")" return SMALL_R_BRACKET;
"\""  return QUOTATION;
"+" return PLUS;
"-" return MINUS;
"\n" return NEWLINE;

{sp} {
    yylval.intVal = 14;
    return REG;
}
{pc} {
    yylval.intVal = 15;
    return REG;
}
{regX} {
    yylval.intVal = yytext[2] - '0';
    return REG; 
}
{reg1X} {
    yylval.intVal = yytext[3] - '0' + 10;
    return REG; 
}
{status} {
    yylval.intVal = 0;
    return STATUS;
}
{handler} {
    yylval.intVal = 1;
    return HANDLER;
}
{cause} {
    yylval.intVal = 2;
    return CAUSE;
}
{symbol} {
    yylval.stringVal = new string(yytext);
    return SYMBOL;
}
{bin} {
    yylval.intVal = stol(yytext, nullptr, 2);
    return BIN;
}
{dec} {
    yylval.intVal = atol(yytext);
    return DEC;
}
{hex} {
    yylval.intVal = stol(yytext, nullptr, 16);
    return HEX;
}

{text} {
    yylval.stringVal = new string(yytext);
    return TEXT;
}

{comment} {
    yylval.stringVal = new string(yytext);
    return COMMENT;
}

{blank}

. {
    cout << "Can't parse [" << yytext << "]" << endl;
}

%%