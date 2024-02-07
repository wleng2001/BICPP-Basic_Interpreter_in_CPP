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
        #if arduino

        #else
        }catch(std::bad_alloc){
            delete e;
            throw;
        #endif
        }catch(notParsed){
            delete e;
            throw;
        }catch(...){
            delete e;
            throw;
        };
        #if arduino
        String value;
        #else
        string value;
        #endif
        if(eVV.type == 'i'){
            #if arduino
            value = String(eVV.valueI);
            #else
            value = to_string(eVV.valueI);
            #endif
        }else{
            if(eVV.type == 'n'){
                #if arduino
                value = String(eVV.valueN);
                #else
                value = to_string(eVV.valueN);
                #endif
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
        #if arduino

        #else
        }catch(std::bad_alloc){
            delete e;
            throw;
        #endif
        }
        return vV;
    }
};

#endif