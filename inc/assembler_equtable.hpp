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
    RelocationTable::RelocationTableLine* reloline;
    EquSymbolReference(string s, RelocationTable::RelocationTableLine* l): symbol(s), reloline(l){}
  };
public:
  static map<string, EquTableLine*> equTable;

  static vector<EquSymbolReference*> equReferences;

  static SymbolTable *symbolTable;

  static void handleNewEquEntry(string symbol, vector<string> expression);

  static void resolveEquSymbols();

  static bool resolveSingleSymbol(string symbol);

  static void handleEquReference(string symbol, RelocationTable::RelocationTableLine *line);

  static void fixEquRelocations();
};

#endif