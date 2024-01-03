#include "variables.h"

string variables::convertHexToDecimal(string data){
    if(data[0] == '0'){
        int length=data.length();
        int value = 0;
        if(data[length-1]=='H'){
            data = data.substr(1, length-2);
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
            data = to_string(value);
            return data;
        }
    }else{
        return data;
    }
}

bool variables::isInt(string &text, uint8_t length){
    for(uint8_t i = 0; i<length; i++){
        if(isdigit(text[i]))
            continue;
        else
            return false;
    }
    return true;
}

bool variables::isNum(string &text, uint8_t length){
    bool isPointer = false;
    for(uint8_t i = 0; i <length; i++){
        char c = text[i];
        if(isdigit(c))
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

void variables::addVariable(string &variableName, string &value){
    uint8_t variableNameLength = variableName.length();
    uint8_t valueLength = value.length();
    char lastChar = variableName[variableNameLength-1];
    if(lastChar=='%'){
        if(isInt(value, valueLength)){
            _VLI[variableName].isArray = false;
            _VLI[variableName].value.reserve(1);
            _VLI[variableName].value[0] = stoi(value);
            return;
        }
        else{
            errorFunc("Error: value assigment to variable isn't int (var: "+variableName+" value: "+value+")");
            *error = true;
            return;
        }
    }else{
        if(lastChar=='$'){
            _VLS[variableName].isArray = false;
            _VLS[variableName].value.reserve(1);
            _VLS[variableName].value[0] = value;
            return;
        }

        if(lastChar==')'){
            errorFunc("Error: array support not added yet");
        }

        if(isNum(value, valueLength)){
            _VLN[variableName].isArray = false;
            _VLN[variableName].value.reserve(1);
            _VLN[variableName].value[0] = stof(value);
            return;
        }
        else{
            errorFunc("Error: value assigment to variable isn't num (var: "+variableName+" value: "+value+")");
            *error = true;
            return;
        }
    }
}

void variables::readVariable(string *variableName, value *var){
    if(_VLI.find(*variableName)!=_VLI.end()){
        var->type = 'i';
        var->valueI = _VLI[*variableName].value[0];
        return;
    }
    if(_VLN.find(*variableName)!=_VLN.end()){
        var->type = 'n';
        var->valueN = _VLN[*variableName].value[0];
        return;
    }
    if(_VLS.find(*variableName)!=_VLS.end()){
        var->type = 'i';
        var->valueS = _VLS[*variableName].value[0];
        return;
    }
}