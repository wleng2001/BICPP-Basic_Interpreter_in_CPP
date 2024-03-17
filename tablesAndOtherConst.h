#ifndef tablesAndOtheConst_h
#define tablesAndOtherConst_h
#include "config.h"

char mathematicChar[] = {'+', '-', '*','^', '/', '<', '>', '=','[',']', '!'};

char specialChar[] = {
  '%', '$'
};

const char* reservedName[] = {
  "NOT",
  "OR",
  "AND",
  "XOR",
  "IMP",
  "EQV",
  "THEN",
  //"ELSE"
};

bool isMathematicChar(char c){
  for(uint8_t i = 0; i<10; i++)
    if(c == mathematicChar[i]){
      return true;
    }
  return false;
}
#endif
