#ifndef parser_h
#define parser_h

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <math.h>
#include <map>
#include "expressions.h"
#include "expressions.cpp"
#include "statements.h"
#include "text_analyzer.h"

using namespace std;

class parser{
    string _input;
    size_t _position;
    void (*errorFunc)(string input);
    string (*_inputFunc)();
    void (*_printFunc)(string *input);
    variables *_vM;

    public:

    parser(string &input, void (*errorFunction)(string input), string (*inputFunction)(), void printFunction(string *input), variables *variableMemory);

    uint8_t parserPosition(){
        return uint8_t(_position);
    }

    void skipWhiteSpace(); //pomija spacje itp.

    char lookAhead(); //zwraca następny znak po spacjach

    expressions* parseExpressions();
    expressions* parseCommand();
    expressions* parseStatements();

    bool parseRem(string statement, bool parsed);
    bool parseLet(string statement, bool parsed);
    bool parseInput(string statement, bool parsed);

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
    expressions* parseVariable(string &s, bool returnName = false);
    expressions* parseParen();
};

#endif