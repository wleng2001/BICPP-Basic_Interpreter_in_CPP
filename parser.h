#ifndef parser_h
#define parser_h

#include "expressions.h"
#include "statements.h"
#include "text_analyzer.h"

using namespace std;

class parser{
    #if arduino
    String _input;
    #else
    string _input;
    #endif
    size_t _position;
    #if arduino
    void (*errorFunc)(String input);
    String (*_inputFunc)();
    void (*_printFunc)(String *input);
    String returnString(expressions *e);
    #else
    void (*errorFunc)(string input);
    string (*_inputFunc)();
    void (*_printFunc)(string *input);
    string returnString(expressions *e);
    #endif
    variables *_vM;
    programMemorySupport *_pMS;
    unsigned int *_programLine;
    void setProgramLine(unsigned int programLine);
    bool valueBiggerThan0(variableValue *vV);
    public:

    #if arduino
    parser(String &input, void (*errorFunction)(String input), String (*inputFunction)(), void printFunction(String *input), variables *variableMemory, programMemorySupport *pMS, unsigned int *programLine);
    parser( void (*errorFunction)(String input), String (*inputFunction)(), void printFunction(String *input), variables *variableMemory, programMemorySupport *pMS);
    void addInput(String &input);
    #else
    parser(string &input, void (*errorFunction)(string input), string (*inputFunction)(), void printFunction(string *input), variables *variableMemory, programMemorySupport *pMS, unsigned int *programLine);
    parser( void (*errorFunction)(string input), string (*inputFunction)(), void printFunction(string *input), variables *variableMemory, programMemorySupport *pMS);
    void addInput(string &input);
    #endif

    void addProgramLine(unsigned int *programLine);

    uint8_t parserPosition(){
        return uint8_t(_position);
    }

    void skipWhiteSpace(); //pomija spacje itp.

    char lookAhead(); //zwraca następny znak po spacjach

    expressions* parseExpressions();
    expressions* parseStatements(bool execute = 1);

    #if arduino
    bool parseRun(String statement, bool parsed, bool execute = 1);
    bool parseRem(String statement, bool parsed, bool execute = 1);
    bool parseLet(String statement, bool parsed, bool execute = 1);
    bool parseInput(String statement, bool parsed, bool execute = 1);
    bool parsePrint(String statement, bool parsed, bool execute = 1);
    bool parseClear(String statement, bool parsed, bool execute = 1);
    bool parseGoto(String statement, bool parsed, bool execute = 1);
    bool parseIf(String statement, bool parsed, bool execute = 1);
    #else
    bool parseRun(string statement, bool parsed, bool execute = 1);
    bool parseRem(string statement, bool parsed, bool execute = 1);
    bool parseLet(string statement, bool parsed, bool execute = 1);
    bool parseInput(string statement, bool parsed, bool execute = 1);
    bool parsePrint(string statement, bool parsed, bool execute = 1);
    bool parseClear(string statement, bool parsed, bool execute = 1);
    bool parseGoto(string statement, bool parsed, bool execute = 1);
    bool parseIf(string statement, bool parsed, bool execute = 1);
    #endif

    expressions* parseFunction();
    expressions* parseLogical();
    expressions* parseNot();
    expressions* parseRelation();
    expressions* parseConcatenation();
    expressions* parseRange();
    expressions* parseSum();
    expressions* parseMult();
    expressions* parseTerm();
    expressions* parseLiteral();
    expressions* parseConstant();
    expressions* parseLogicalVariable();
    #if arduino
    expressions* parseVariable(String &s, bool returnName = false);
    #else
    expressions* parseVariable(string &s, bool returnName = false);
    #endif
    expressions* parseParen();
};

#endif
