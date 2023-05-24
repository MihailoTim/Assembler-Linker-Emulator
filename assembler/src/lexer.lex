%{
    #include <iostream>
    #include <vector>
    #include "../tmp/parser.h"
    using namespace std;
    extern int yylex();
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

%%
".global" { return GLOBAL; }
".extern" { return EXTERN; }
".section" { return SECTION; }
".word" { return WORD; }
".skip" { return SKIP; }
".ascii" { return ASCII;}
".equ" { return EQU; }
".end" { return END; }
"halt" { return HALT; }
"int" { return INT; }
"iret" { return IRET; }
"call" { return CALL; }
"ret" { return RET; }
"jmp" { return JMP; }
"beq" { return BEQ; }
"bne" { return BNE; }
"bgt" { return BGT; }
"push" { return PUSH; }
"pop" { return POP; }
"xchg" { return XCHG; }
"add" { return ADD; }
"sub" { return SUB; }
"mul" { return MUL; }
"div" { return DIV; }
"not" { return NOT; }
"and" { return AND; }
"or" { return OR; }
"xor" { return XOR; }
"shl" { return SHL; }
"shr" { return SHR; }
"ld" { return LD; }
"st" { return ST; }
"csrrd" { return CSRRD;}
"csrwr" { return CSRWR;}
"," { return COMMA; }
":" { return COLON; }
";" { return SEMICOLON;}
"%" { return PERCENT; }
"$" { return DOLLAR; }
"[" { return MID_L_BRACKET; }
"]" { return MID_R_BRACKET; }
"\"" { return QUOTATION;}
"+" {return PLUS;}
"-" {return MINUS;}

"sp" {
    yylval.number = 14;
    return REG;
}
"pc" {
    yylval.number = 15;
    return REG;
}
%r[0-9] {
    yylval.number = yytext[2] - '0';
    return REG; 
}
%r1[0-5] {
    yylval.number = 10 + (yytext[2] - '0');
    return REG; 
}
"%status" {
    yylval.symbol = "status";
    return STATUS;
}
"%handler" {
    yylval.symbol = "handler";
    return HANDLER;
}
"%cause" {
    yylval.symbol = "cause";
    return CAUSE;
}
{symbol} {
    yylval.symbol = strdup(yytext);
    return SYMBOL;
}
{bin} {
    yylval.number = stoi(yytext, nullptr, 2);
    return BIN;
}
{dec} {
    yylval.number = atoi(yytext);
    return DEC;
}
{hex} {
    yylval.number = stoi(yytext, nullptr, 16);
    return HEX;
}

{text} {
    yylval.symbol = strdup(yytext);
    return TEXT;
}

{blank}

{comment}

. {
    cout << "Can't parse [" << yytext << "]" << endl;
}

%%