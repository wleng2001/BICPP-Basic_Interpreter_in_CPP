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
    bool *error;
    void (*errorFunc)(string input);
    uint8_t *_parserPosition;

    public:

    parser(string input, bool *error, void (*errorFunction)(string input), uint8_t *parserPosition);

    uint8_t parserPosition(){
        return uint8_t(_position);
    }

    void skipWhiteSpace(); //pomija spacje itp.

    char lookAhead(); //zwraca następny znak po spacjach

    expressions* parseExpressions();
    expressions* parseCommand();
    expressions* parseStatements();

    expressions* parseRem(string statement);
    expressions* parseLet(string statement);
    expressions* parseInput(string statement);

    expressions* parseFunction();
    expressions* parseLogical();
    expressions* parseNot();
    expressions* parseRelation();
    expressions* parseConcatenation();
    expressions* parseRange();
    expressions* parseSum();
    expressions* parseMult();
    expressions* parseTerm();
    expressions* parseConstant();
    expressions* parseLogicalVariable();
    expressions* parseVariable(string &s);
    expressions* parseParen();
};

#endif