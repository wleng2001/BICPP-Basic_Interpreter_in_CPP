#include "basic.h"

void basic::printError(string errorText, char c, string *input, unsigned int *lineNumber){
    string text = errorText+" (char: "+to_string(c);
    if(*lineNumber>0){
        text+=" line: "+to_string(*lineNumber);
    }
    text+=+"): "+*input;
    errorFunc(text);
}

basic::basic(void (*printFunction)(string *text), void (*errorFunction)(string text), string (*inputFunction)()){
    printFunc = printFunction;
    errorFunc = errorFunction;
    txt_an.addErrorFuntion(errorFunc);
    _varMemory.addErrorFunction(errorFunc);
    pMS.addErrorFunction(errorFunc);
    inputFunc = inputFunction;
    txt_an.error = &error;
    _varMemory.addErrorVariable(&error);
}

string basic::run(string *input){
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
    errorFunc(*input);
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
    return programLoop(input);
}

string basic::programLoop(string *input){
    for(unsigned int i=0; i<=pMS.maxLine; i++){
        if(_interruptExist==1){
                if(interruptFunc()==1){
                    errorFunc("User interrupt");
                    return "";
            }
        }
        string text;
        if(i>0){
            if(!pMS._pM.count(i)){
                continue;
            }else{
                text=pMS._pM[i];
            }
        }else{
            text = *input;
        }
        parser pars(text, errorFunc, inputFunc, printFunc, &_varMemory, &pMS, &i);
        variableValue vV;
        try{ 
            expressions* e = pars.parseExpressions();
            vV = e->eval(&_varMemory);
        }catch(notParsed){
            printError("Error: not parsed", pars.parserPosition(), &text, &i);
            return "";
        }catch(variableNotFound){
            printError("", pars.parserPosition()-1, &text, &i);
            return ""; 
        }catch(wrongStringRange){
            printError("Error: incorrect range for substring operation", pars.parserPosition()-1, &text, &i);
            return ""; 
        }catch(wrongType){
            printError("Error: wrong type", pars.parserPosition()-1, &text, &i);
            return ""; 
        }catch(wrongRange){
            printError("Error: wrong substring range", pars.parserPosition()-1, &text, &i);
            return ""; 
        }catch(wrongOperator){
            printError("Error: wrong operator", pars.parserPosition()-1, &text, &i);
            return ""; 
        }catch(variableNameAbsence){
            return "";
        }catch(wrongVariableName){
            printError("Error: variable can't begin by number", pars.parserPosition()-1, &text, &i);
            return "";
        }catch(notNumber){
            printError("Error: it's not numeric constant", pars.parserPosition()-1, &text, &i);
            return "";
        }catch(tooManyArg){
            printError("Error: to many arguments given", pars.parserPosition()-1, &text, &i);
            return "";
        }catch(std::bad_alloc){
            printError("Error: overflow", pars.parserPosition(), &text, &i);
            return "";
        }
        if(pMS.maxLine==0 && i>0){
            return "";
        }
        if(i==0){
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
        }
    }
}