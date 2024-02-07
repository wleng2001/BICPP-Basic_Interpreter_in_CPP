#ifndef expressions_h
#define expressions_h

#include "config.h"

#if arduino
#include <Arduino.h>
#else
#include <iostream>
#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#include <cmath>
#include <algorithm>
#include <map>
#endif
#include <math.h>
#include "variables.h"
#include "variables.cpp"
#include "text_analyzer.h"
#include "programMemorySuppport.h"

using namespace std;

#if arduino
void (*expErrorFunc)(String input);
#else
void (*expErrorFunc)(string input);
#endif

class expressions{

    public:
    virtual variableValue eval(variables *vM) = 0;
    virtual ~expressions(){};
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
    #if arduino
    String value;
    #else
    string value;
    #endif

    public:
    #if arduino
    constantS(String v): value(v){}
    #else
    constantS(string v): value(v){}
    #endif

    virtual variableValue eval(variables *vM){
        variableValue vV;
        vV.type = 's';
        vV.valueS = value;
        return vV;
    }
};

class logicalSymbol : public expressions{
    #if arduino
    String value;
    #else
    string value;
    #endif

    public:
    #if arduino
    logicalSymbol(String v): value(v){}
    #else
    logicalSymbol(string v): value(v){}
    #endif

    virtual variableValue eval(variables *vM){
        variableValue vV;
        vV.type = 'l';
        vV.valueS = value;
        return vV;
    }
};

#if arduino
String convertToString(variableValue *v){
    String value = "";
#else
string convertToString(variableValue *v){
    string value = "";
#endif
    if(v->type == 's'){
        value = v->valueS;
        return value;
    }else{
        if(v->type == 'i'){
            #if arduino
            value = String(v->valueI);
            #else
            value = to_string(v->valueI);
            #endif
            return value;
        }else{
            if(v->type == 'n'){
                #if arduino
                value = String(int(v->valueN));
                #else
                value = to_string(int(v->valueN));
                #endif
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
    return value;
}

class logicalOperator : public expressions{
    #if arduino
    String symbol;
    #else
    string symbol;
    #endif
    expressions* left, *right;
    public:
    #if arduino
    logicalOperator(String s , expressions* l, expressions* r) : symbol(s), left(l), right(r){};
    #else
    logicalOperator(string s , expressions* l, expressions* r) : symbol(s), left(l), right(r){};
    #endif

    virtual ~logicalOperator(){
        delete left;
        delete right;
        delete &symbol;
    }

    variableValue eval(variables *vM){
        #if debug
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
        }
        #if debug
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
    #if arduino
    String symbol;
    #else
    string symbol;
    #endif
    expressions *right;

    public:

    #if arduino
    notOperator(String s, expressions* r) : symbol(s), right(r){};
    #else
    notOperator(string s, expressions* r) : symbol(s), right(r){};
    #endif

    virtual ~notOperator(){
        delete right;
        delete &symbol;
    }

    variableValue eval(variables *vM){
        variableValue vV;
        variableValue rVV = right->eval(vM);
        int rightValue;
        #if debug
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
        #if debug
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
    expressions *left, *right;
    #if arduino
    String symbol="";
    #else
    string symbol="";
    #endif

    public:
    #if arduino
    relationOperator(String s, expressions* l, expressions* r) : symbol(s), left(l), right(r){};
    #else
    relationOperator(string s, expressions* l, expressions* r) : symbol(s), left(l), right(r){};
    #endif

    virtual ~relationOperator(){
        delete left;
        delete right;
        delete &symbol;
    }

    variableValue eval(variables *vM){
        variableValue vV;
        variableValue rVV = right->eval(vM);
        variableValue lVV = left->eval(vM);
        #if arduino
        String rightValue;
        String leftValue;
        #else
        string rightValue;
        string leftValue;
        #endif
        uint8_t rightLength;
        uint8_t leftLength;
        #if debug
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
        #if debug
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
        if(symbol == "=" || symbol == "=="){
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
        return vV;
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

        #if debug
            expErrorFunc("concatenationOperators");
        #endif

        if(lVV.type == 's' && rVV.type == 's'){
            vV.type = 's';
            switch(symbol){
                case '&':
                    vV.valueS = lVV.valueS + rVV.valueS;
                    return vV;
                default:
                    expErrorFunc("Error: can't " + 
                    #if arduino
                    String(symbol) 
                    #else
                    string(1, symbol)
                    #endif
                    + " for strings");
            }
        }else{
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
        #if debug
        expErrorFunc("substringOperation");
        #endif

        variableValue vV;
        variableValue tSSVV = tSS->eval(vM);
        variableValue sRVV = sR->eval(vM);
        variableValue eRVV = eR->eval(vM);
        if(tSSVV.type == 's' && sRVV.type == 'i' && eRVV.type == 'i'){
            if(sRVV.valueI>eRVV.valueI || eRVV.valueI>=int(tSSVV.valueS.length()))
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

        #if debug
            expErrorFunc("binaryOperator: "+string(1,symbol));
        #endif
        if(lVV.type == 's' || rVV.type == 's'){
            throw wrongType();
        }
        
        if(lVV.type == 'i' && rVV.type == 'i'){
            vV.type = 'i';
            switch(symbol){
                case '*':
                    vV.valueI = lVV.valueI * rVV.valueI;
                    return vV;
                case '/':
                    if(rVV.valueI!=0){
                        vV.valueI = lVV.valueI / rVV.valueI;
                        return vV;
                    }else{
                        throw notParsed();
                    }
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
                    if(rVV.valueN!=0){
                        vV.valueN = lVV.valueN / rVV.valueN;
                        return vV;
                    }else{
                        throw notParsed();
                    }
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
                    throw wrongOperator();
            }
            return vV;
        }
        throw variableNotFound();
        return vV;
    }
};

class variable : public expressions{
    #if arduino
    String name = "";
    #else
    string name = "";
    #endif

    public:
    #if arduino
    variable(String n) : name(n){};
    #else
    variable(string n) : name(n){};
    #endif

    virtual ~variable(){
        delete &name;
    }

    virtual variableValue eval(variables *vM){
        variableValue vV;
        #if debug
        expErrorFunc("variable");
        #endif 
        if(!vM->readVariable(&name, &vV)){
            throw variableNotFound(); //errorClasses.h
        }
        #if debug
        expErrorFunc("variableValue: "+name+"\t"+to_string(vV.valueI)+'\t'+to_string(vV.valueN)+'\t'+vV.valueS);
        #endif
        return vV;
    }
};

#endif
