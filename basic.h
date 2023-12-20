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

    bool (*interruptFunc)();
    bool _interruptExist=0;
    Memory _variableMemory;

    string *text;
    text_analyzer txt_an;
    variables var;
    

    public:

    bool addInterruptFunc(bool (*func)()){
        interruptFunc = func;
        _interruptExist = 1;
    }

    string run(string *input){
        if(*input==""){
            return "";
        }
        if(_interruptExist==1){
            if(interruptFunc()==1){
                return "INTERRUPT";
            }
        }
        txt_an.reform_input(*input);

        #if debug
        cout << *input << endl;
        #endif

        if(!programMemorySupport::checkAndSave(*input))
            return "";

        #if debug
        cout << "Listuje..." << endl;
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
        #endif

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