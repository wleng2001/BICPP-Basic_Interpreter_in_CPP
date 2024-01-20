#include "text_analyzer.h"
#include "text_analyzer.cpp"
#include "expressions.h"
#include "expressions.cpp"
#include "parser.h"
#include "parser.cpp"
#include "programMemorySuppport.h"
#include "programMemorySuppport.cpp"
#include <iostream>
#include <algorithm>

#define _basicVersion "A0.02"

using namespace std;

class basic{
    private:

    bool error = 0;
    bool (*interruptFunc)();
    bool _interruptExist=0;
    void (*printFunc)(string text);
    void (*errorFunc)(string text);
    string (*inputFunc)();
    variables _varMemory;
    string *text;
    text_analyzer txt_an;
    programMemorySupport pMS;

    public:

    basic(void (*printFunction)(string text), void (*errorFunction)(string text), string (*inputFunction)()){
        printFunc = printFunction;
        errorFunc = errorFunction;
        txt_an.addErrorFuntion(errorFunc);
        _varMemory.addErrorFunction(errorFunc);
        pMS.addErrorFunction(errorFunc);
        inputFunc = inputFunction;
        txt_an.error = &error;
        _varMemory.addErrorVariable(&error);
    }

    bool addInterruptFunc(bool (*func)()){
        interruptFunc = func;
        _interruptExist = 1;
    }

    string run(string *input){
        error = 0;
        if(*input==""){
            return "";
        }
        if(_interruptExist==1){
            if(interruptFunc()==1){
                return "INTERRUPT";
            }
        }
        txt_an.reform_input(*input);
        if(error) return "";
        #if debug
        cout << *input << endl;
        #endif

        if(!pMS.checkAndSave(*input))
            return "";

        #if debug
        #endif
        if( input->find("LIST")!=string::npos){
            string output = "";
            string partOutput = "";
            for(auto i = 0; i<=pMS.maxLine; i++){
                partOutput = "";
                if(pMS._pM.count(i)){
                    partOutput += pMS._pM[i];
                    output += to_string(i) + '\t' + partOutput + '\n';
                    partOutput = "";
                }
                if(_interruptExist==1){
                    if(interruptFunc()==1){
                        errorFunc("User interrupt");
                        return "";
                }
        }
            }
            return output;
        }

        parser pars(*input, &error, errorFunc);
        try{ 
            expressions* e = pars.parseExpressions();
            variableValue vV = e->eval(&_varMemory);
            switch(vV.type){
                case 's':
                    return vV.valueS;
                case 'i':
                    return to_string(vV.valueI);
                case 'n':
                    return to_string(vV.valueN);
                case 'N':
                    return "";
            }
        }catch(notParsed){
            errorFunc("Error: not parsed (char: " + to_string(pars.parserPosition()) + "): "+*input);
            return "";
        }catch(variableNotFound){
            errorFunc("(char: "+to_string(pars.parserPosition()-1)+"): "+*input);
            return ""; 
        }catch(wrongStringRange){
            errorFunc("Error: incorrect range for substring operation (char: "+ to_string(pars.parserPosition()-1) +"): "+*input);
            return ""; 
        }catch(wrongType){
            errorFunc("Error: wrong type (char: "+to_string(pars.parserPosition()-1)+"): "+*input);
            return ""; 
        }catch(wrongRange){
            errorFunc("Error: wrong substring range (char: "+to_string(pars.parserPosition()-1)+"): "+*input);
            return ""; 
        }catch(wrongOperator){
            errorFunc("Error: wrong operator (char: "+to_string(pars.parserPosition()-1)+"): "+*input);
            return ""; 
        }catch(variableNameAbsence){
            return "";
        }catch(wrongVariableName){
            errorFunc("Error: variable can't begin by number (char: "+to_string(pars.parserPosition()-1)+"): "+*input);
            return "";
        }catch(notNumber){
            errorFunc("Error: it's not numeric constant (char: "+to_string(pars.parserPosition()-1)+"): "+*input);
            return "";
        }
        
    }
};