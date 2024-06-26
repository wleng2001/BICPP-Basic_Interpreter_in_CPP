#include "basic.h"

#if arduino
void basic::printError(String errorText, char c, String *input, unsigned int *lineNumber){
#else
void basic::printError(string errorText, char c, string *input, unsigned int *lineNumber){
#endif
    #if arduino
    String text = errorText+" (char: "+String(c);
    #else
    string text = errorText+" (char: "+to_string(c);
    #endif
    if(*lineNumber>0){
        #if arduino
        text+=" line: "+String(*lineNumber);
        #else
        text+=" line: "+to_string(*lineNumber);
        #endif
    }
    text+=+"): "+*input;
    errorFunc(text);
}

#if arduino
basic::basic(void (*printFunction)(String *text), void (*errorFunction)(String text), String (*inputFunction)()){
#else
basic::basic(void (*printFunction)(string *text), void (*errorFunction)(string text), string (*inputFunction)()){
#endif
    printFunc = printFunction;
    errorFunc = errorFunction;
    txt_an.addErrorFuntion(errorFunc);
    _varMemory.addErrorFunction(errorFunc);
    pMS.addErrorFunction(errorFunc);
    inputFunc = inputFunction;
    txt_an.error = &error;
    _varMemory.addErrorVariable(&error);
}

#if arduino
String basic::run(String *input){
#else
string basic::run(string *input){
#endif
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
    
    #if arduino
    if( input->indexOf("LIST")!= -1){
    #else
    if( input->find("LIST")!= string::npos){
    #endif
        #if arduino
        String output = "";
        String partOutput = "";
        #else
        string output = "";
        string partOutput = "";
        #endif
        for(auto i = 0; i<=pMS.maxLine; i++){
            partOutput = "";
            if(pMS._pM.count(i)){
                partOutput += pMS._pM[i];
                #if arduino
                output += String(i) + String('\t') + partOutput + String('\n');
                #else
                output += to_string(i) + '\t' + partOutput + '\n';
                #endif
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

#if arduino
String basic::programLoop(String *input){
#else
string basic::programLoop(string *input){
#endif
    parser pars( errorFunc, inputFunc, printFunc, &_varMemory, &pMS);
    for(unsigned int i=0; i<=pMS.maxLine; i++){
        variableValue vV;
        if(_interruptExist==1){
                if(interruptFunc()==1){
                    errorFunc("User interrupt");
                    return "";
            }
        }
        #if arduino
        String text;
        #else
        string text;
        #endif
        if(i>0){
            if(!pMS._pM.count(i)){
                continue;
            }else{
                text=pMS._pM[i];
            }
        }else{
            text = *input;
        }
        
        pars.addInput(text);
        pars.addProgramLine(&i);
        try{ 
            expressions* e = pars.parseExpressions();
            vV = e->eval(&_varMemory);
            delete e;
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
        #if arduino
        #else
        }catch(std::bad_alloc){
            printError("Error: overflow", pars.parserPosition(), &text, &i);
            return "";
        #endif
        }catch(wrongSyntax){
            printError("Error: wrong syntax", pars.parserPosition(), &text, &i);
            return "";
        }catch(...){
            printError("Error: unrecognized exceptrion", pars.parserPosition(), &text, &i);
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
                    #if arduino
                    return String(vV.valueI);
                    #else
                    return to_string(vV.valueI);
                    #endif
                case 'n':
                    #if arduino
                    return String(vV.valueN);
                    #else
                    return to_string(vV.valueN);
                    #endif
                case 'N':
                    return "";
            }
        }
    }

    return "";
}
