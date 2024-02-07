#ifndef statements_h
#define statements_h
#include "expressions.h"
using namespace std;

class letStatement : public expressions{
    #if arduino
    String varName;
    #else
    string varName;
    #endif
    expressions *e;

    public:
    #if arduino
    letStatement(String variableName, expressions *right): varName(variableName), e(right){};
    #else
    letStatement(string variableName, expressions *right): varName(variableName), e(right){};
    #endif
    virtual ~letStatement(){
        delete e;
    }
    variableValue eval(variables *vM){
        #if debug
        expErrorFunc("LET "+varName);
        #endif
        variableValue vV;
        variableValue eVV;
        try{
            eVV = e->eval(vM);
        }catch(variableNotFound){
            delete e;
            throw;
        }catch(std::bad_alloc){
            delete e;
            throw;
        }catch(notParsed){
            delete e;
            throw;
        };
        #if arduino
        String value;
        #else
        string value;
        #endif
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
        #if debug
        expErrorFunc("LET "+varName+" = "+value);
        #endif
        try{
            vM->addVariable(varName, value);
        }catch(wrongType){
            delete e;
            throw wrongType();
        }catch(std::bad_alloc){
            delete e;
            throw;
        }
        return vV;
    }
};

#endif