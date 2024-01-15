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

class logicalSymbol : public expressions{
    string value;

    public:
    logicalSymbol(string v): value(v){}

    virtual variableValue eval(variables *vM){
        variableValue vV;
        vV.type = 'l';
        vV.valueS = value;
        return vV;
    }
};

string convertToString(variableValue *v){
    string value = "";
    if(v->type == 's'){
        value = v->valueS;
        return value;
    }else{
        if(v->type == 'i'){
            value = to_string(v->valueI);
            return value;
        }else{
            if(v->type == 'n'){
                value = to_string(int(v->valueN));
                return value;
            }else{
                throw wrongType();
                return value;
            }
        }
    }
}

int convertToInt(variableValue *v){
    int value;
    if(v->type == 'i'){
        return v->valueI;
    }else{
        if(v->type == 'n'){
            value = int(v->valueN);
            return value;
        }else{
            throw wrongType();
        }
    }
}

class logicalOperator : public expressions{
    string symbol;
    expressions* left, *right;
    public:
    logicalOperator(string s , expressions* l, expressions* r) : symbol(s), left(l), right(r){};

    virtual ~logicalOperator(){
        delete left;
        delete right;
    }

    variableValue eval(variables *vM){
        #ifdef debug
        expErrorFunc("logicalOperator");
        #endif
        variableValue vV;
        variableValue lVV = left->eval(vM);
        variableValue rVV = right->eval(vM);
        int leftValue;
        int rightValue;
        try{
            leftValue = convertToInt(&lVV);
            rightValue = convertToInt(&rVV);
        }catch(wrongType()){
            delete left;
            delete right;
            throw wrongType();
            return vV;
        }
        #ifdef debug
        expErrorFunc(to_string(leftValue) + " " + symbol + " " + to_string(rightValue));
        #endif
        vV.type = 'i';

        if(symbol == "OR"){
            if(leftValue>0 || rightValue>0){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        if(symbol == "AND"){
            if(leftValue>0 && rightValue>0){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }

        if(symbol == "XOR"){
            if((leftValue>0 || rightValue>0) && (leftValue!=rightValue)){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }

        if(symbol == "IMP"){
            if((leftValue>0 && rightValue>0)||(leftValue==0 && rightValue==0)||(leftValue==0 && rightValue>0)){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }

        if(symbol == "EQV"){
            if((leftValue>0 && rightValue>0)||(leftValue==0 && rightValue==0)){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        throw wrongOperator();
        return vV;
    }
};

class notOperator : public expressions{
    string symbol;
    expressions *right;

    public:

    notOperator(string s, expressions* r) : symbol(s), right(r){};

    virtual ~notOperator(){
        delete right;
    }

    variableValue eval(variables *vM){
        variableValue vV;
        variableValue rVV = right->eval(vM);
        int rightValue;
        #ifdef debug
        expErrorFunc("notOperator");
        #endif
        try{
            rightValue = convertToInt(&rVV);
        }catch(wrongType()){
            delete right;
            delete &rVV;
            throw wrongType();
            return vV;
        }

        vV.type = 'i';
        #ifdef debug
        expErrorFunc(symbol+" "+to_string(rightValue));
        #endif
        if(symbol == "NOT"){
            if(rightValue>0){
                vV.valueI = 0;
            }else{
                vV.valueI = 1;
            }
            return vV;
        }else{
            throw wrongOperator();
            return vV;
        }
    }
};

class relationOperator : public expressions{
    expressions* left, *right;
    string symbol="";
    public:
    relationOperator(string s, expressions* l, expressions* r) : symbol(s), left(l), right(r){};

    virtual ~relationOperator(){
        delete left;
        delete right;
    }

    variableValue eval(variables *vM){
        variableValue vV;
        variableValue rVV = right->eval(vM);
        variableValue lVV = left->eval(vM);
        string rightValue;
        string leftValue;
        uint8_t rightLength;
        uint8_t leftLength;
        #ifdef debug
            expErrorFunc("relationOperator");
        #endif

        try{
            rightValue = convertToString(&rVV);
            rightLength = rightValue.length();

            leftValue = convertToString(&lVV);
            leftLength = leftValue.length();

            if((lVV.type!='s' && lVV.type!='l')&&(rVV.type!='s' && rVV.type!='l')){
                if(leftLength>rightLength){
                    for(uint8_t i =0; i<leftLength-rightLength; i++){
                        rightValue = '0'+rightValue;
                    }
                }else{
                    if(leftLength<rightLength){
                        for(uint8_t i =0; i<leftLength-rightLength; i++){
                            rightValue = '0'+rightValue;
                    }
                    }

                }
            }
        }catch(...){
            delete &vV;
            delete &lVV;
            delete &rVV;
            throw wrongType();
        }
        vV.type = 'i';
        #ifdef debug
        expErrorFunc("Left: "+leftValue+"\tRight: "+rightValue+"\tOperator: "+symbol);
        #endif
        if(symbol == ">"){
            if(leftValue>rightValue){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        if(symbol == ">="){
            if(leftValue>=rightValue){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        if(symbol == "<"){
            if(leftValue<rightValue){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        if(symbol == "<=" || symbol == "=<"){
            if(leftValue<=rightValue){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        if(symbol == "="){
            if(leftValue==rightValue){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        if(symbol == "<>"){
            if(leftValue!=rightValue){
                vV.valueI = 1;
                return vV;
            }else{
                vV.valueI = 0;
                return vV;
            }
        }
        throw wrongOperator();
    }
};

class concatenationOperator : public expressions{
    expressions* left, *right;
    char symbol;
    char type = 'N';
    public:
    concatenationOperator(char s, expressions* l, expressions* r) : symbol(s), left(l), right(r){}

    virtual ~concatenationOperator(){
        delete left;
        delete right;
    }

    virtual variableValue eval(variables *vM){
        variableValue vV;
        variableValue rVV = right->eval(vM);
        variableValue lVV = left->eval(vM);

        #ifdef debug
            expErrorFunc("concatenationOperators");
        #endif

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
        }else{
            *expError = 1;
            throw wrongType();
            return vV;
        }

        

    }
};

class substringOperation : public expressions{
    expressions* tSS, *sR, *eR;

    public:
    substringOperation(expressions* toSubString, expressions* startRange, expressions* endRange): tSS(toSubString), sR(startRange), eR(endRange){}

    virtual ~substringOperation(){
        delete tSS;
        delete sR;
        delete eR;
    }

    virtual variableValue eval(variables *vM){
        #ifdef debug
        expErrorFunc("substringOperation");
        #endif

        variableValue vV;
        variableValue tSSVV = tSS->eval(vM);
        variableValue sRVV = sR->eval(vM);
        variableValue eRVV = eR->eval(vM);
        if(tSSVV.type == 's' && sRVV.type == 'i' && eRVV.type == 'i'){
            if(sRVV.valueI>eRVV.valueI || eRVV.valueI>=tSSVV.valueS.length())
                throw wrongRange();
            else{
                vV.type = 's';
                for(uint8_t i = sRVV.valueI; i<=eRVV.valueI; i++){
                    vV.valueS+=tSSVV.valueS[i];
                }
            }
        }else{
            throw wrongType();
        }
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
        if(lVV.type == 's' || rVV.type == 's'){
            throw wrongType();
            return vV;
        }
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
                    throw wrongOperator();
            }
            return vV;
        }

        if((lVV.type == 'i' || lVV.type == 'n') && (rVV.type == 'i' || rVV.type == 'n')){
            if(lVV.type == 'i'){
                lVV.valueN = lVV.valueI;
            }

            if(rVV.type == 'i'){
                rVV.valueN = rVV.valueI;
            }

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
                    throw wrongOperator();
            }
            return vV;
        }
        *expError = 1;
        throw variableNotFound();
        return vV;
    }
};

class variable : public expressions{
    string name;

    public:
    variable(string n) : name(n){};

    virtual variableValue eval(variables *vM){
        variableValue vV;
        #ifdef debug
        expErrorFunc("variable");
        #endif 
        if(!vM->readVariable(&name, &vV)){
            *expError = 1;
            throw variableNotFound(); //erroClasses.h
        }
        #ifdef debug
        expErrorFunc("variableValue: "+name+" "+to_string(vV.valueI)+'\t'+to_string(vV.valueI)+'\t'+vV.valueS);
        #endif
        return vV;
    }
};

#endif