#include "variables.h"

#if arduino
void variables::addErrorFunction(void (*errorFunction)(String text)){
#else
void variables::addErrorFunction(void (*errorFunction)(string text)){
#endif
    errorFunc = errorFunction;
}

void variables::addErrorVariable(bool *error){
    this->error = error;
}

#if arduino
String variables::convertHexToDecimal(String data){
#else
string variables::convertHexToDecimal(string data){
#endif
    if(isdigit(data[0]) || isalpha(data[0])){
        uint8_t length=data.length();
        int value = 0;
        if(data[length-1]=='H'){
            #if arduino
            data = data.substring(0, length-1);
            #else
            data = data.substr(0, length-1);
            #endif
            errorFunc(data);
            length = data.length();
            for(int i = length-1; i>=0 ; i--){
                char i_value = data[i];
                switch(i_value){
                    case 'A':
                        value+=A*pow(16, length-1-i);
                        break;
                    case 'B':
                        value+=B*pow(16, length-1-i);
                        break;
                    case 'C':
                        value+=C*pow(16, length-1-i);
                        break;
                    case 'D':
                        value+=D*pow(16, length-1-i);
                        break;
                    case 'E':
                        value+=E*pow(16, length-1-i);
                        break;
                    case 'F':
                        value+=F*pow(16, length-1-i);
                        break;
                    default:
                        value+=(data[i]-48)*pow(16, length-1-i);
                        break;
                }
            }
            #if arduino
            data = String(value);
            #else
            data = to_string(value);
            #endif
            errorFunc(data);
            return data;
        }
    }
    return data;
}

bool variables::isHexLetter(char c){
    c = toupper(c);
    if((c>='A' && c<='F'))
        return true;
    else
        return false;
}

#if arduino
bool variables::isInt(String &text, uint8_t length){
#else
bool variables::isInt(string &text, uint8_t length){
#endif
    for(uint8_t i = 0; i<length; i++){
        char c = text[i];
        if(isdigit(c) || isHexLetter(c) || (c=='H' && i==length-1) || (c=='-' && i==0))
            continue;
        else
            return false;
    }
    return true;
}

#if arduino
bool variables::isNum(String &text, uint8_t length){
#else
bool variables::isNum(string &text, uint8_t length){
#endif
    bool isPointer = false;
    for(uint8_t i = 0; i <length; i++){
        char c = text[i];
        if(isdigit(c))
            continue;
        if(c=='-' && i==0)
            continue;
        if(c=='.' && !isPointer){
            isPointer = true;
            continue;
        }else{
            return false;
        }
    }
    return true;
}

void variables::clearMemory(){
    _VLI.clear();
    _VLN.clear();
    _VLS.clear();
}

#if arduino
void variables::addVariable(String &variableName, String &value){
#else
void variables::addVariable(string &variableName, string &value){
#endif
    uint8_t variableNameLength = variableName.length();
    uint8_t valueLength = value.length();
    char lastChar = variableName[variableNameLength-1];
    #if arduino
    #else
    try{
    #endif
        switch(lastChar){
        case '%':
            if(isInt(value, valueLength) || isNum(value, valueLength)){
                _VLI[variableName].isArray = false;
                _VLI[variableName].value.reserve(1);
                if(isInt(value, valueLength))
                    #if arduino
                    _VLI[variableName].value[0] = convertHexToDecimal(value).toInt();
                    #else
                    _VLI[variableName].value[0] = stoi(convertHexToDecimal(value));
                    #endif
                else
                    #if arduino
                    _VLI[variableName].value[0] = value.toInt();
                    #else
                    _VLI[variableName].value[0] = stof(value);
                    #endif
                return;
            }
            else{
                errorFunc("Error: value assigment to variable isn't int (var: "+variableName+" value: "+value+")");
                *error = true;
                throw wrongType();
                return;
            }
        case '$':
            _VLS[variableName].isArray = false;
            _VLS[variableName].value.clear();
            _VLS[variableName].value.reserve(1);
            _VLS[variableName].value.push_back(value);
            return;
        case ')':
            errorFunc("Error: array support not added yet");
            return;
        default:
            if(isNum(value, valueLength)){
                _VLN[variableName].isArray = false;
                _VLN[variableName].value.reserve(1);
                #if arduino
                _VLN[variableName].value[0] = value.toFloat();
                #else
                _VLN[variableName].value[0] = stof(value);
                #endif
                return;
            }
            else{
                errorFunc("Error: value assigment to variable isn't num (var: "+variableName+" value: "+value+")");
                *error = true;
                throw wrongType();
                return;
            }
        }
    #if arduino

    #else
    }catch(std::bad_alloc){
        throw;
    }
    #endif
    
}

#if arduino
bool variables::readVariable(String *variableName, variableValue *var){
#else
bool variables::readVariable(string *variableName, variableValue *var){
#endif
    #if debug
    errorFunc("readVariable: "+*variableName);
    #endif
    var->type = 'N';
    var->valueI = 0;
    var->valueN = 0;
    var->valueS = "";
    if(_VLI.find(*variableName)!=_VLI.end()){
        var->type = 'i';
        var->valueI = _VLI[*variableName].value[0];
        var->valueN = 0;
        var->valueS = "";
        return true;
    }
    if(_VLN.find(*variableName)!=_VLN.end()){
        var->type = 'n';
        var->valueN = _VLN[*variableName].value[0];
        return true;
    }
    if(_VLS.find(*variableName)!=_VLS.end()){
        var->type = 's';
        var->valueS += _VLS[*variableName].value[0];
        return true;
    }
    errorFunc("ERROR: can't find variable: "+*variableName);
    return false;
}
