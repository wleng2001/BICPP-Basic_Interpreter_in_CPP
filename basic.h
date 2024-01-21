#include "text_analyzer.h"
#include "text_analyzer.cpp"
#include "expressions.h"
#include "expressions.cpp"
#include "parser.h"
#include "parser.cpp"
#include "programMemorySuppport.h"
#include "programMemorySuppport.cpp"
#include <iostream>
#include <algorithm>

#define _basicVersion "A0.02"

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
    string (*inputFunc)();
    variables _varMemory;
    string *text;
    text_analyzer txt_an;
    programMemorySupport pMS;

    public:

    basic(void (*printFunction)(string *text), void (*errorFunction)(string text), string (*inputFunction)());

    bool addInterruptFunc(bool (*func)()){
        interruptFunc = func;
        _interruptExist = 1;
    }

    string run(string *input);
    string programLoop(string *input);
};