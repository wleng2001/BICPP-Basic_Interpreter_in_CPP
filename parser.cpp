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
    expressions* e = parseLogical();
    if(lookAhead() == EOS)
        return e;
    else
        delete e;
        throw notParsed();
}

expressions* parser::parseLogical(){
    #ifdef debug
    errorFunc("parseLogical: "+to_string(_position));
    #endif
    expressions* e = parseRelation();
    char c = lookAhead();
    string s = "";
    #ifdef debug
    errorFunc("parseLogical2: "+to_string(_position));
    #endif
    while(c=='N' || c=='O' || c=='A' || c=='X' || c=='I' || c == 'E'){
        s.push_back(c);
        _position++;
        s.push_back(lookAhead());
        if(s=="OR"){
            _position++;
            e = new logicalOperator(s, e, parseRelation());
            s="";
        }else{
            #ifdef debug
            errorFunc("parseLogical4: "+to_string(_position));
            #endif
            _position++;
            s.push_back(lookAhead());
            _position++;
            e = new logicalOperator(s, e, parseRelation());
            s="";
        }
        c=lookAhead();

    }
    return e;
}

expressions* parser::parseRelation(){
    #ifdef debug
    errorFunc("parseRelation: "+to_string(_position));
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
    errorFunc("parseConcatenation: "+to_string(_position));
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
    errorFunc("parseRange: "+to_string(_position));
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
    errorFunc("parseSum: "+to_string(_position));
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
    errorFunc("parseMult: "+to_string(_position));
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
    errorFunc("parseTerm: "+to_string(_position));
    #endif
    char c = lookAhead();
    if(isdigit(c) || c =='.' || c=='"'){
        return parseConstant();
    }else if(isalpha(c)){
        return parseLogicalVariable();
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
    errorFunc("parseConstant: "+to_string(_position));
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

expressions* parser::parseLogicalVariable(){
    string s = "";
    #ifdef debug
    errorFunc("parserLogicalVariable: "+to_string(_position));
    #endif
    int position = _position;
    while(isalpha(_input[_position])){
        s.push_back(_input[_position]);
        for(uint8_t i = 0; i<(sizeof(reservedName)/sizeof(reservedName[0])); i++){
            if(s==reservedName[i]){
                _position = position;
                return new logicalSymbol(s);
            }
        }
        _position++;
    }
    return parseVariable(s);
}

expressions* parser::parseVariable(string &s){
    #ifdef debug
    errorFunc("parseVariable: "+to_string(_position));
    #endif
    while(isalnum(_input[_position])){
        s.push_back(_input[_position]);
        _position++;
    }
    return new variable(s);
}

expressions* parser::parseParen(){
    _position++;
    #ifdef debug
    errorFunc("parseParen: "+to_string(_position));
    #endif
    expressions* e = parseLogical();
    if(lookAhead() == ')'){
        _position++;
        return e;
    }else{
        delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
}