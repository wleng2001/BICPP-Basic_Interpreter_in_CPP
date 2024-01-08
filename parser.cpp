#include "parser.h"

const char EOS = 0;

parser::parser(string input, bool *error, void (*errorFunction)(string input)) : _input(input), _position(0), error(error), errorFunc(errorFunction){
    input.push_back(EOS); //umieszcza strażnika na końcu
    expError = error;
    expErrorFunc = errorFunction;
}

void parser::skipWhiteSpace(){
    while(isspace(_input[_position])) //jeśli znak to biały znak
        _position++;
}

bool parser::isMathematicChar(char c){
  for(uint8_t i = 0; i<10; i++)
    if(c == mathematicChar[i]){
      return true;
    }else
      return false;
}

char parser::lookAhead(){
    skipWhiteSpace();
    return _input[_position];
}

expressions* parser::parseExpressions(){
    expressions* e = parseSum();
    if(lookAhead() == EOS)
        return e;
    else
        delete e;
        throw notParsed();
}

expressions* parser::parseSum(){
    expressions* e = parseMult();
    char c = lookAhead();

    try{
        while(c == '+' || c == '-'){
            _position ++;
            e = new binaryOperator(c, e, parseMult());
            c = lookAhead();
        }
    }catch(notParsed){
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseMult(){
    expressions* e = NULL;

    try{
        e = parseTerm();
        char c = lookAhead();

        while(c == '*' || c == '/' || c == '^'){
            _position++;
            e = new binaryOperator(c, e, parseTerm());
            c = lookAhead();
        }
    }catch(notParsed){
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseTerm(){
    char c = lookAhead();
    if(isdigit(c) || c =='.' || c=='"'){
        return parseConstant();
    }else if(isalpha(c)){
        return parseVariable();
    }else if(c =='('){
        return parseParen();
    }else
        throw notParsed();
}

expressions* parser::parseConstant(){
    string n = "";
    bool isDecimalSeparator = false;
    bool isNumber = false;
    if(_input[_position]!='"'){
        errorFunc("Jestem w while1");
        while(isdigit(_input[_position]) || (_input[_position]=='.')){
            isNumber = true;
            n+= _input[_position];
            if(_input[_position]=='.'){
                if(isDecimalSeparator == false)
                    isDecimalSeparator = true;
                else{
                    *this->error = 1;
                    this->errorFunc("Error: to much pointer: "+_input);
                }
            }
            _position++;
        }
    }else{
        while(_input[_position]!='"'){
            errorFunc("Jestem w while2");
            n+=_input[_position];
            _position++;
        }
    }
    if(isNumber == true){
        if(isDecimalSeparator==false)
            return new constant(stoi(n));
        else
            return new constantN(stof(n));
    }else
        return new constantS(n);
}

expressions* parser::parseVariable(){
    string s;
    while(isalnum(_input[_position])){
        s.push_back(_input[_position]);
        _position++;
    }
    return new variable(s);
}

expressions* parser::parseParen(){
    _position++;
    expressions* e = parseSum();
    if(lookAhead() == ')'){
        _position++;
        return e;
    }else{
        delete e;
        throw notParsed();
    }
}