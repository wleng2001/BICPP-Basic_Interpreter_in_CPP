#ifndef tablesAndOtheConst_h
#define tablesAndOtherConst_h

char mathematicChar[] = {'+', '-', '*','^', '/', '<', '>', '=','[',']'};

char specialChar[] = {
  '%', '$'
};

const char* reservedName[6] = {
  "NOT",
  "OR",
  "AND",
  "XOR",
  "IMP",
  "EQV"
};

bool isMathematicChar(char c){
  for(uint8_t i = 0; i<10; i++)
    if(c == mathematicChar[i]){
      return true;
    }
  return false;
}
#endif