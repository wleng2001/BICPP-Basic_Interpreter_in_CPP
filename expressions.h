#ifndef expressions_h
#define expressions_h

#include <iostream>
#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#include <cmath>
#include <algorithm>
#include <math.h>
#include <map>
#include "variables.h"
#include "variables.cpp"

using namespace std;

//#define debug true

bool *expError;
void (*expErrorFunc)(string input);

class expressions{

    public:
    virtual variableValue eval(variables *vM) = 0;
    virtual ~expressions(){}
};

class constant : public expressions{
    int value;

    public:
    constant(int v) : value(v){

    }

    virtual variableValue eval(variables *vM){
        variableValue vV;
        vV.type = 'i';
        vV.valueI = value;
        vV.valueN = value;
        return vV;
    }


};

class constantN : public expressions{
    float value;

    public:
    constantN(float v) : value(v){

    }

    virtual variableValue eval(variables *vM){
        variableValue vV;
        vV.type = 'n';
        vV.valueN = value;
        return vV;
    }

    

};

class constantS : public expressions{
    string value;

    public:
    constantS(string v): value(v){

    }

    virtual variableValue eval(variables *vM){
        variableValue vV;
        vV.type = 's';
        vV.valueS = value;
        return vV;
    }
};

class binaryOperator : public expressions{
    char symbol;
    expressions* left, * right;
    char type = 'N';

    public:
    binaryOperator(char s, expressions* l, expressions* r) : symbol(s), left(l), right(r){
    }

    virtual ~binaryOperator(){
        delete left;
        delete right;
    }

    virtual variableValue eval(variables *vM){
        variableValue vV;
        variableValue lVV = left -> eval(vM);
        variableValue rVV = right -> eval(vM);

        #ifdef debug
            expErrorFunc("binaryOperator");
        #endif

        if(lVV.type == 'i' && rVV.type == 'i'){
            vV.type = 'i';
            switch(symbol){
                case '*':
                    vV.valueI = lVV.valueI * rVV.valueI;
                    return vV;
                case '/':
                    vV.valueI = lVV.valueI / rVV.valueI;
                    return vV;
                case '+':
                    vV.valueI = lVV.valueI + rVV.valueI;
                    return vV;
                case '-':
                    vV.valueI = lVV.valueI - rVV.valueI;
                    return vV;
                case '^':
                    vV.valueI = pow(lVV.valueI, rVV.valueI);
                    return vV;
                default:
                    *expError = 1;
                    expErrorFunc("Error: can't "+string(1, symbol)+" for int");
            }
            return vV;
        }

        if((lVV.type == 'i' || lVV.type == 'n') && (rVV.type == 'i' || rVV.type == 'n')){

            vV.type = 'n';
            switch(symbol){
                case '*':
                    vV.valueN = lVV.valueN * rVV.valueN;
                    return vV;
                case '/':
                    vV.valueN = lVV.valueN / rVV.valueN;
                    return vV;
                case '+':
                    vV.valueN = lVV.valueN + rVV.valueN;
                    return vV;
                case '-':
                    vV.valueN = lVV.valueN - rVV.valueN;
                    return vV;
                case '^':
                    vV.valueN = pow(lVV.valueN, rVV.valueN);
                    return vV;
                default:
                    *expError = 1;
                    expErrorFunc("Error: can't "+string(1, symbol)+" for numeric");
            }
            return vV;
        }

        if(lVV.type == 's' && rVV.type == 's'){
            vV.type = 's';
            switch(symbol){
                case '&':
                    vV.valueS = lVV.valueS + rVV.valueS;
                    return vV;
                default:
                    *expError = 1;
                    expErrorFunc("Error: can't " + string(1, symbol) + " for strings");
            }
        }

        return vV;
    }
};

class variableNotFound{

};

class variable : public expressions{
    string name;

    public:
    variable(string n) : name(n){
        
    }

    virtual variableValue eval(variables *vM){
        variableValue vV;
        if(!vM->readVariable(&name, &vV))
            throw variableNotFound();
        return vV;
    }
};

#endif