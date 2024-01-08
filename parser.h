#ifndef parser_h
#define parser_h

#include <iostream>
#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#include <cmath>
#include <windows.h> //unistd.h dla linux
#include <cstdlib> //biblioteka, aby działały komendy sytemowe np: system("cls");
#include <time.h>
#include <conio.h> //wprowadzanie danych bez entera
#include <fstream> //biblioteka do obsługi plików
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

    public:

    parser(string input, bool *error, void (*errorFunction)(string input));

    void skipWhiteSpace(); //pomija spacje itp.

    bool isMathematicChar(char c);

    char lookAhead(); //zwraca następny znak po spacjach

    expressions* parseExpressions();
    expressions* parseSum();
    expressions* parseMult();
    expressions* parseTerm();
    expressions* parseConstant();
    expressions* parseVariable();
    expressions* parseParen();
};

class notParsed{};

#endif