#include "text_analyzer.h"
#include "text_analyzer.cpp"
#include "expressions.h"
#include "parser.h"
#include "parser.cpp"
#include "programMemorySuppport.h"
#include "programMemorySuppport.cpp"
#include <iostream>
#include <algorithm>

#define _basicVersion "A0.03"

using namespace std;

class basic{
    private:

    bool error = 0;
    bool toExecute;
    int programIterator;
    bool (*interruptFunc)();
    bool _interruptExist=0;
    void (*printFunc)(string *text);
    void (*errorFunc)(string text);
    #if arduino
    String (*inputFunc)();
    #else
    string (*inputFunc)();
    #endif
    variables _varMemory;
    string *text;
    text_analyzer txt_an;
    programMemorySupport pMS;
    #if arduino
    void printError(String errorText, char c, String *input, unsigned int *lineNumber);
    #else
    void printError(string errorText, char c, string *input, unsigned int *lineNumber);
    #endif
    public:

    basic(void (*printFunction)(string *text), void (*errorFunction)(string text), string (*inputFunction)());

    bool addInterruptFunc(bool (*func)()){
        interruptFunc = func;
        _interruptExist = 1;
    }
    #if arduino
    String run(String *input);
    String programLoop(String *input);
    #else
    string run(string *input);
    string programLoop(string *input);
    #endif
};