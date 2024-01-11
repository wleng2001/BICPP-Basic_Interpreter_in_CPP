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

    void skipWhiteSpace(); //pomija spacje itp.

    bool isMathematicChar(char c);

    char lookAhead(); //zwraca następny znak po spacjach

    expressions* parseExpressions();
    expressions* parseConcatenation();
    expressions* parseRange();
    expressions* parseSum();
    expressions* parseMult();
    expressions* parseTerm();
    expressions* parseConstant();
    expressions* parseVariable();
    expressions* parseParen();
};

#endif