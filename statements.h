#ifndef statements_h
#define statements_h
#include "expressions.h"
using namespace std;

class letStatement : public expressions{
    string varName;
    expressions *e;

    public:
    letStatement(string variableName, expressions *right): varName(variableName), e(right){};
    virtual ~letStatement(){
        delete e;
    }
    variableValue eval(variables *vM){
        #ifdef debug
        expErrorFunc("LET "+varName);
        #endif
        variableValue vV;
        variableValue eVV = e->eval(vM);
        string value;
        if(eVV.type == 'i'){
            value = to_string(eVV.valueI);
        }else{
            if(eVV.type == 'n'){
                value = to_string(eVV.valueN);
            }else{
                if(eVV.type == 's'){
                    value = eVV.valueS;
                }else{
                    throw wrongType();
                    delete e;
                    return vV;
                }
            }
        }
        #ifdef debug
        expErrorFunc("LET "+varName+" = "+value);
        #endif
        try{
            vM->addVariable(varName, value);
        }catch(wrongType){
            delete e;
            throw wrongType();
        }
        return vV;
    }
};

#endif