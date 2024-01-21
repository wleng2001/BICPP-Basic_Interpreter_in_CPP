#include "basic.h"

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
        parser pars(text, errorFunc, inputFunc, printFunc, &_varMemory, &i);
        variableValue vV;
        try{ 
            expressions* e = pars.parseExpressions();
            vV = e->eval(&_varMemory);
        }catch(notParsed){
            errorFunc("Error: not parsed (char: " + to_string(pars.parserPosition()) + "): "+text);
            return "";
        }catch(variableNotFound){
            errorFunc("(char: "+to_string(pars.parserPosition()-1)+"): "+text);
            return ""; 
        }catch(wrongStringRange){
            errorFunc("Error: incorrect range for substring operation (char: "+ to_string(pars.parserPosition()-1) +"): "+text);
            return ""; 
        }catch(wrongType){
            errorFunc("Error: wrong type (char: "+to_string(pars.parserPosition()-1)+"): "+text);
            return ""; 
        }catch(wrongRange){
            errorFunc("Error: wrong substring range (char: "+to_string(pars.parserPosition()-1)+"): "+text);
            return ""; 
        }catch(wrongOperator){
            errorFunc("Error: wrong operator (char: "+to_string(pars.parserPosition()-1)+"): "+text);
            return ""; 
        }catch(variableNameAbsence){
            return "";
        }catch(wrongVariableName){
            errorFunc("Error: variable can't begin by number (char: "+to_string(pars.parserPosition()-1)+"): "+text);
            return "";
        }catch(notNumber){
            errorFunc("Error: it's not numeric constant (char: "+to_string(pars.parserPosition()-1)+"): "+text);
            return "";
        }catch(tooManyArg){
            errorFunc("Error: to many arguments given: "+text);
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