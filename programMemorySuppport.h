#ifndef programMemorySupport_h
#define programMemorySupport_h
#include <iostream>
#include <algorithm>
#include <map>

//#define debug true

using namespace std;

#if arduino
typedef std::map < int, String > programMemory;
#else
typedef std::map < int, string > programMemory;
#endif

class programMemorySupport{
    int _position=0;
    bool checkMathematicSymbol(char input);
    #if arduino
    int takeDigits(String &input);
    void skipWhiteSpace(String &input);
    unsigned int quantityOfSpecificChar(String &input, char c);
    void (*_errorFunc)(String input);
    #else
    int takeDigits(string &input);
    void skipWhiteSpace(string &input);
    unsigned int quantityOfSpecificChar(string &input, char c);
    void (*_errorFunc)(string input);
    #endif
    programMemorySupport *vMS;

    public:

    programMemory _pM;
    int maxLine = 0;

    programMemorySupport(){};
    #if arduino
    programMemorySupport(programMemorySupport *higherInstance, void (*errorFunc)(String input)) : vMS(higherInstance),  _errorFunc(errorFunc){};
    void addErrorFunction(void (*errorFunction)(String input)){
        _errorFunc = errorFunction;
    };
    void memoryClear();
    bool isToExecute(String &input); //zwraca true jeśli input to równanie a nie linijka do zapisania w pamięci
    bool checkAndSave(String &input);
    #else
    programMemorySupport(programMemorySupport *higherInstance, void (*errorFunc)(string input)) : vMS(higherInstance),  _errorFunc(errorFunc){};
    void addErrorFunction(void (*errorFunction)(string input)){
        _errorFunc = errorFunction;
    };
    void memoryClear();
    bool isToExecute(string &input); //zwraca true jeśli input to równanie a nie linijka do zapisania w pamięci
    bool checkAndSave(string &input);
    #endif

};

#endif
