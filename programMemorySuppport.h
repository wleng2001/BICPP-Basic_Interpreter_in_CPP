#ifndef programMemorySupport_h
#define programMemorySupport_h
#include <iostream>
#include <algorithm>
#include <map>

//#define debug true

using namespace std;

typedef std::map < int, string > programMemory;

class programMemorySupport{
    int _position=0;
    bool checkMathematicSymbol(char input);
    int takeDigits(string &input);
    void skipWhiteSpace(string &input);
    unsigned int quantityOfSpecificChar(string &input, char c);
    void (*_errorFunc)(string input);
    programMemorySupport *vMS;

    public:

    programMemory _pM;
    int maxLine = 0;

    programMemorySupport(){};
    programMemorySupport(programMemorySupport *higherInstance, void (*errorFunc)(string input)) : vMS(higherInstance),  _errorFunc(errorFunc){};
    void addErrorFunction(void (*errorFunction)(string input)){
        _errorFunc = errorFunction;
    };
    void memoryClear();
    bool isToExecute(string &input); //zwraca true jeśli input to równanie a nie linijka do zapisania w pamięci
    bool checkAndSave(string &input);

};

#endif
