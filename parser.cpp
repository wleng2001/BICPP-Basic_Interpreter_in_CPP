#include "parser.h"

const char EOS = 0;

parser::parser(string input, bool *error, void (*errorFunction)(string input), uint8_t *parserPosition) : _input(input), _position(0), error(error), errorFunc(errorFunction), _parserPosition(parserPosition){
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
    expressions* e = parseRelation();
    if(lookAhead() == EOS)
        return e;
    else
        delete e;
        throw notParsed();
}

expressions* parser::parseRelation(){
    #ifdef debuf
    errorFunc("parseRelation");
    #endif
    expressions* e = parseConcatenation();
    char c = lookAhead();
    string s="";
    s=string(1,c);
    while(c=='>' || c=='<' || c=='='){
        _position++;
        c = _input[_position];
        if(c=='='||c=='<'||c=='>'){
            s+=c;
            _position++;
            try{
                e = new relationOperator(s, e, parseConcatenation());
            }catch(...){
                delete e;
                throw notParsed();
            }
        }else{
            try{
                e = new relationOperator(s, e, parseConcatenation());
            }catch(...){
                delete e;
                throw notParsed();
            }
        }
        c = lookAhead();
    }
    return e;
}

expressions* parser::parseConcatenation(){
    #ifdef debug
    errorFunc("parseConcatenation");
    #endif
    expressions* e = parseRange();
    char c = lookAhead();
    try{
        while(c =='&'){
            _position++;
            e = new concatenationOperator(c, e, parseRange());
            c = lookAhead();
        }
    }catch(...){
        delete e;

    }
    return e;
}

expressions* parser::parseRange(){
    #ifdef debug
    errorFunc("parseRange");
    #endif
    uint8_t position = _position;
    expressions* e = parseSum();
    char c = lookAhead();

    try{
        if(c == '['){
            _position++;
            expressions* startRange = parseSum();
            c = lookAhead();
            if(c==':'){
                _position++;
                expressions* endRange = parseSum();
                c = lookAhead();
                if(c==']'){
                    e = new substringOperation(e, startRange, endRange);
                    _position++;
                }else{
                    delete e;
                    delete startRange;
                    delete endRange;
                    *_parserPosition = _position;
                    throw wrongStringRange();
                }
            }else{
                delete e;
                delete startRange;
                *_parserPosition = _position;
                throw wrongStringRange();
            }
        }else{
            delete e;
            _position = position;
            return parseSum();
        }
    }catch(...){
        delete e;
        throw notParsed();
    }

    #ifdef debug
    errorFunc("parseRange");
    #endif

    return e;
}

expressions* parser::parseSum(){
    #ifdef debug
    errorFunc("parseSum");
    #endif
    expressions* e = parseMult();
    char c = lookAhead();

    try{
        while(c == '+' || c == '-'){
            _position++;
            e = new binaryOperator(c, e, parseMult());
            c = lookAhead();
        }
    }catch(...){
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseMult(){
    #ifdef debug
    errorFunc("parseMult");
    #endif
    expressions* e = NULL;

    try{
        e = parseTerm();
        char c = lookAhead();

        while(c == '*' || c == '/' || c == '^'){
            _position++;
            e = new binaryOperator(c, e, parseTerm());
            c = lookAhead();
        }
    }catch(...){
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseTerm(){
    #ifdef debug
    errorFunc("parseTerm");
    #endif
    char c = lookAhead();
    if(isdigit(c) || c =='.' || c=='"'){
        return parseConstant();
    }else if(isalpha(c)){
        return parseVariable();
    }else if(c =='('){
        return parseParen();
    }else
        *_parserPosition = _position;
        throw notParsed();
}

expressions* parser::parseConstant(){
    string n = "";
    bool isDecimalSeparator = false;
    bool isNumber = false;
    #ifdef debug
    errorFunc("parseConstant");
    #endif
    if(_input[_position]!='"'){
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
        _position++;
        while(_input[_position]!='"'){
            n+=_input[_position];
            _position++;
        }
        _position++;
    }
    if(isNumber == true){
        if(isDecimalSeparator==false)
            return new constant(stoi(n));
        else
            return new constantN(stof(n));
    }else{
        return new constantS(n);
    }
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
    expressions* e = parseRelation();
    if(lookAhead() == ')'){
        _position++;
        return e;
    }else{
        delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
}