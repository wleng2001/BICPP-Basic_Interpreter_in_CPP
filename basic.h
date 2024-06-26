#include "text_analyzer.h"
#include "text_analyzer.cpp"
#include "expressions.h"
#include "parser.h"
#include "parser.cpp"
#include "programMemorySuppport.h"
#include "programMemorySuppport.cpp"

#if arduino

#else
#include <iostream>
#include <algorithm>
#endif

#define _basicVersion "A0.04"

using namespace std;

class basic{
    private:

    bool error = 0;
    bool toExecute;
    int programIterator;
    bool (*interruptFunc)();
    bool _interruptExist=0;
    #if arduino
    void (*printFunc)(String *text);
    void (*errorFunc)(String text);
    String (*inputFunc)();
    String *text;
    #else
    void (*printFunc)(string *text);
    void (*errorFunc)(string text);
    string (*inputFunc)();
    string *text;
    #endif
    variables _varMemory;
    text_analyzer txt_an;
    programMemorySupport pMS;
    #if arduino
    void printError(String errorText, char c, String *input, unsigned int *lineNumber);
    #else
    void printError(string errorText, char c, string *input, unsigned int *lineNumber);
    #endif
    public:

    #if arduino
    basic(void (*printFunction)(String *text), void (*errorFunction)(String text), String (*inputFunction)());
    #else
    basic(void (*printFunction)(string *text), void (*errorFunction)(string text), string (*inputFunction)());
    #endif

    void addInterruptFunc(bool (*func)()){
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
