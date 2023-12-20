#ifndef expressions_h
#define expressions_h

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
#include "variables.h"
#include "variables.cpp"

using namespace std;

typedef map <string, int> Memory;

class expressions{
    public:
    virtual float eval(Memory& m) = 0;
    virtual ~expressions(){}
};

class constant : public expressions{
    float valueI;

    public:
    constant(float v) : valueI(v){

    }

    virtual float eval(Memory& m){
        return valueI;
    }


};

class binaryOperator : public expressions{
    char symbol;
    expressions* left, * right;

    public:
    binaryOperator(char s, expressions* l, expressions* r) : symbol(s), left(l), right(r){
    }

    virtual ~binaryOperator(){
        delete left;
        delete right;
    }

    virtual float eval(Memory& m){
        switch (symbol){
            case '*': return left->eval(m) * right->eval(m);
            case '+': return left->eval(m) + right->eval(m);
            case '-': return left->eval(m) - right->eval(m);
            case '/': return left->eval(m) / right->eval(m);
        }
    }
};

class variableNotFound{

};

class variable : public expressions{
    string name;

    public:
    variable(string n) : name(n){
        
    }

    virtual float eval(Memory& m){
        Memory::iterator it = m.find(name);
        if(it == m.end())
            throw variableNotFound();
        return it->second;
    }
};

#endif