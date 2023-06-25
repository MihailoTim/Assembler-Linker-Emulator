#include "../inc/assembler_assemblyLine.hpp"
#include "../inc/assembler_symbolTable.hpp"
#include "../inc/assembler_relocationTable.hpp"
#include <map>
#include <vector>
#include <iostream>
#include <set>
using namespace std;

#ifndef _EQU_TABLE_H
#define _EQU_TABLE_H

class EquTable{
  struct EquTableArgument{
    bool isSymbol;
    string stringVal;
    int intVal;
    bool isSigned;
    EquTableArgument(string sval, int ival, bool isSym, bool sign): stringVal(sval), intVal(ival), isSymbol(isSym), isSigned(sign){}
  };
  struct EquTableLine{
    vector<EquTableArgument*> args;
  };
  struct EquSymbolReference{
    string symbol;
    RelocationTableLine* reloline;
    bool displFitCheck;
    string section;
    size_t locationCounter;
    EquSymbolReference(string s, RelocationTableLine* l, string sctn, bool check = false, size_t lc = 0): symbol(s), reloline(l), displFitCheck(check), section(sctn), locationCounter(lc){}
  };
public:
  static map<string, EquTableLine*> equTable;

  static vector<EquSymbolReference*> equReferences;

  static SymbolTable *symbolTable;

  static void handleNewEquEntry(string symbol, vector<string> expression);

  static void resolveEquSymbols();

  static bool resolveSingleSymbol(string symbol);

  static void handleEquReference(string symbol, RelocationTableLine *line, string section, bool displFitCheck = false, size_t locationCounter = 0);

  static void fixEquRelocations();
};

#endif