#ifndef programMemorySupport_h
#define programMemorySupport_h
#include <iostream>
#include <algorithm>
#include <map>

//#define debug false

using namespace std;

typedef std::map < int, string > programMemory;

class programMemorySupport{
    int _position=0;
    bool checkMathematicSymbol(char input);
    int takeDigit(string &input);
    void skipWhiteSpace(string &input);
    unsigned int quantityOfSpecificChar(string &input, char c);
    static char mathematic_char[];

    protected:

    programMemory _pM;
    int maxLine = 0;

    public:

    bool isToExecute(string &input); //zwraca true jeśli input to równanie a nie linijka do zapisania w pamięci
    bool checkAndSave(string &input);

};

#endif
