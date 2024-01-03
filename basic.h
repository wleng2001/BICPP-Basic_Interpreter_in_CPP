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

//#define debug true

using namespace std;

class basic : public programMemorySupport{
    private:

    bool error = 0;
    bool (*interruptFunc)();
    bool _interruptExist=0;
    void (*printFunc)(string text);
    void (*errorFunc)(string text);
    string (*inputFunc)();
    Memory _variableMemory;
    string *text;
    text_analyzer txt_an;

    public:

    basic(void (*printFunction)(string text), void (*errorFunction)(string text), string (*inputFunction)()){
        printFunc = printFunction;
        errorFunc = errorFunction;
        txt_an.addErrorFuntion(errorFunction);
        inputFunc = inputFunction;
        txt_an.error = &error;
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

        if(!programMemorySupport::checkAndSave(*input))
            return "";

        #if debug
        #endif
        if(*input == "LIST"){
            string output = "";
            string partOutput = "";
            for(auto i = 0; i<=programMemorySupport::maxLine; i++){
                partOutput = "";
                if(programMemorySupport::_pM.count(i)){
                    partOutput += programMemorySupport::_pM[i];
                    output += to_string(i) + '\t' + partOutput + '\n';
                    partOutput = "";
                }
            }
            return output;
        }


        try{
            parser pars(*input);
            expressions* e = pars.parseExpressions();
            return to_string(e->eval(_variableMemory));
        }catch(notParsed){
            errorFunc("ERROR: not parsed");
            return "";
        }catch(variableNotFound){
            errorFunc("ERROR: interpreter doesn't support variable");
            return ""; 
        }
        
    }
};