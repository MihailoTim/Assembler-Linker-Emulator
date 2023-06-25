#include "../inc/assembler_equtable.hpp"
#include "../inc/assembler_secondPass.hpp"
#include "../inc/assembler_assemblyInstruction.hpp"


map<string, EquTable::EquTableLine*> EquTable::equTable;
vector<EquTable::EquSymbolReference*> EquTable::equReferences;
SymbolTable* EquTable::symbolTable = &SymbolTable::getInstance();

void EquTable::handleNewEquEntry(string symbol, vector<string> expression){
  EquTableLine *line = new EquTableLine();
  for(int i=0;i<expression.size();){
    int val = 0;
    string sym = "";
    bool sign = false;
    bool isSym = false;
    if(expression[i] == "+" || expression[i] == "-"){
      try{
        val = stoi(expression[i+1]);
        sym = to_string(val);
        sign = expression[i] == "-";
      }
      catch(exception e){
        sym = expression[i+1];
        sign = expression[i] == "-";
        isSym = true;
      }
      i+=2;
    }
    else{
      try{
        val = stoi(expression[i]);
        sym = to_string(val);
      }
      catch(exception e){
        sym = expression[i];
        isSym = true;
      }
      i++;
    }

    EquTableArgument *arg = new EquTableArgument(sym, val, isSym, sign );
    line->args.push_back(arg);
  }

  equTable.insert(make_pair(symbol, line));
}

void EquTable::resolveEquSymbols(){

  set<string> equSet;

  for(auto it = equTable.begin(); it!=equTable.end(); it++){
    equSet.insert(it->first);
  }

  while(equSet.size() > 0){

    size_t setSize = equSet.size();
    
    for(auto it = equTable.begin(); it!=equTable.end(); it++){
      bool resolved = false;

      if(equSet.count(it->first)){
        resolved = resolveSingleSymbol(it->first);
      }

      if(resolved){
        equSet.erase(it->first);
      }
    }

    if(equSet.size() == setSize){
      throw new Exception("Couldn't resolve .equ directive for symbol: " + *equSet.begin());
    }
  }
}

bool EquTable::resolveSingleSymbol(string symbol){
  EquTableLine *line = equTable[symbol];
  map<int, int> sectionCount;

  int addend = 0;
  int externCount = 0;
  size_t externIndex = -2;

  for(auto arg: line->args){
    if(arg->isSymbol){
      SymbolTable::SymbolTableLine *stline = &symbolTable->symbolTable[arg->stringVal];

      if(stline->ndx == SymbolTable::SymbolSection::EXTERN){
        externCount++;
        externIndex = stline->num;
      }
      else{
        if(stline->type == SymbolTable::SymbolType::EQU_UNRESOLVED)
          return false;
        
        if(arg->isSigned){
          if(stline->type != SymbolTable::SymbolType::EQU_LITERAL)
            sectionCount[stline->ndx]--;
          addend -= stline->value;
        }
        else{
          if(stline->type != SymbolTable::SymbolType::EQU_LITERAL)
            sectionCount[stline->ndx]++;
          addend += stline->value;
        }
      }
    }
    else{
      if(arg->isSigned)
        addend -= arg->intVal;
      else
        addend += arg->intVal;
    }
  }

  int sum = 0;
  size_t section = -1;
  for(auto it = sectionCount.begin(); it!=sectionCount.end(); it++){
    sum += (it->second != 0);
    if(it->second){
      section = it->first;
    }
  }

  if(sum > 1 || externCount > 1 || (sum != 0 && externCount > 0)){
    return false;
  }
  SymbolTable::SymbolTableLine *stline = &symbolTable->symbolTable[symbol];
  stline->value = addend;
  if(externCount == 1){
    stline->type = SymbolTable::SymbolType::EQU_SYMBOL;
    stline->ndx = externIndex;
  }
  else{
    if(section == -1){
      stline->type = SymbolTable::SymbolType::EQU_LITERAL;
    }
    else{
      stline->type = SymbolTable::SymbolType::EQU_SYMBOL;
    }

    stline->ndx = section;
  }

  return true;
}

void EquTable::handleEquReference(string symbol, RelocationTableLine* line, string section, bool displFitCheck, size_t locationCounter){
  equReferences.push_back(new EquSymbolReference(symbol, line, section, displFitCheck, locationCounter));
}

void EquTable::fixEquRelocations(){
  for(auto ref : equReferences){
    SymbolTable::SymbolTableLine *stline = &symbolTable->symbolTable[ref->symbol];
    if(ref->displFitCheck){
      if(stline->type != SymbolTable::SymbolType::EQU_LITERAL){
        throw new Exception("Symbol whose value cannot be determined is being used in regind addressing");
      }
      else{
        if(!SecondPass::canFitInDispl(stline->value, 0)){
          throw new Exception("Symbol value can't be embedded into a 12 bit displacement");
        }
        else{
          SymbolTable::SectionTableLine* sctnline = &symbolTable->sectionTable[ref->section];
			    int displ  = stline->value;
				  string byte3 = AssemblyInstruction::getByte((displ >> 8) & 0xF);
				  string byte4 = AssemblyInstruction::getByte(displ & 0xFF);
				  string replacement = byte3 + byte4;
				  sctnline->content.replace((ref->locationCounter*2 + 5), 3, replacement.substr(1));
			  }
		  }
    }
    else{
      SymbolTable::SectionTableLine* sctnline = &symbolTable->sectionTable[ref->section];
      size_t id = ref->reloline->id;
      RelocationTableLine* reloline = nullptr;
      for(auto it : sctnline->reloTable){
        if(it->id == id)
          reloline = it;
      }
      if(reloline){
        if(stline->type == SymbolTable::SymbolType::EQU_SYMBOL)
          reloline->addend = stline->value;
        reloline->referencedSymbol = (stline->ndx < 0 ? stline->num : stline->ndx);
      }
    }
  }
}