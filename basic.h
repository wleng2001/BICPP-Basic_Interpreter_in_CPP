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

#define quantityOfVariables 100

using namespace std;

class basic : public programMemorySupport{
    private:

    bool error = 0;
    bool (*interruptFunc)();
    bool _interruptExist=0;
    void (*printFunc)(string text);
    string (*inputFunc)();
    Memory _variableMemory;
    string *text;
    variables var;
    text_analyzer txt_an;

    public:

    basic(void (*printFunction)(string text), string (*inputFunction)()){
        printFunc = printFunction;
        txt_an.addPrintFuntion(printFunc);
        inputFunc = inputFunction;
    }

    bool addInterruptFunc(bool (*func)()){
        interruptFunc = func;
        _interruptExist = 1;
    }

    string run(string *input){
        txt_an.error = &error;
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
        cout << "Listuje..." << endl;
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
            return "ERROR: not parsed";
        }catch(variableNotFound){
            return "ERROR: interpreter doesn't support variable";
        }
        
    }
};