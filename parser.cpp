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

char parser::lookAhead(){
    skipWhiteSpace();
    return _input[_position];
}

expressions* parser::parseExpressions(){
    expressions* e;
    e = parseStatements();

    if(lookAhead() == EOS)
        return e;
    else{
        delete e;
        throw notParsed();
    }
}

expressions* parser::parseStatements(){
    #ifdef debug 
    errorFunc("parserStatements: "+to_string(_position));
    #endif
    expressions *e;
    char c = lookAhead();
    string statement;
    try{
        while(isalpha(c)){
            statement.push_back(c);
            _position++;
            c = _input[_position];
        }
        if(c == ' '){
            e = parseLet(statement);
        }else{
            _position = 0;
            return parseFunction();
        }
    }catch(wrongType){
        delete e;
        *_parserPosition = _position;
        throw wrongType();
        return e;
    }catch(notParsed){
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseLet(string statement){
    expressions *e;
    if(statement == "LET"){
        #ifdef debug
        errorFunc("parseLet: "+to_string(_position));
        #endif
        char c = lookAhead();
        string s;
        while(c!='='){
            s.push_back(c);
            _position++;
            c = lookAhead();
        }
        #ifdef debug
        errorFunc("varName: "+s);
        #endif
        _position++;
        if(s!=""){
            try{
                e = new letStatement(s, parseFunction());
                return e;
            }catch(wrongType){
                delete e;
                *_parserPosition = _position;
                throw wrongType();
                return e;
            }catch(notParsed){
                delete e;
                throw notParsed();
            }
        }else{
            *_parserPosition = _position;
            throw variableNameAbsence();
            return e;
        }
    }else{
        delete e;
        return e;
    }
}

expressions* parser::parseFunction(){
    #ifdef debug
    errorFunc("parseFunction: "+to_string(_position));
    #endif
    return parseLogical();
}

expressions* parser::parseLogical(){
    #ifdef debug
    errorFunc("parseLogical: "+to_string(_position));
    #endif
    expressions* e;
    char c;
    string s = "";
    #ifdef debug
    errorFunc("parseLogical2: "+to_string(_position));
    #endif
    try{
        e = parseNot();
        c = lookAhead();
        while( c=='O' || c=='A' || c=='X' || c=='I' || c == 'E'){
            s.push_back(c);
            _position++;
            s.push_back(_input[_position]);
            if(_input[_position+1]==' '){
                if(s=="OR"){
                    _position++;
                    e = new logicalOperator(s, e, parseNot());
                    s="";
                }
            }else{
                _position++;
                s.push_back(_input[_position]);
                _position++;
                if(_input[_position]==' ' && (s == "AND" || s == "XOR" || s == "IMP" || s =="EQV"))
                e = new logicalOperator(s, e, parseNot());
                s="";
            }
            c=lookAhead();

        }
    }catch(wrongOperator()){
        delete e;
        *_parserPosition = _position;
        throw wrongOperator();
    }catch(wrongType()){
        delete e;
        *_parserPosition = _position;
        cout << _position << endl;
        throw wrongType();
    }catch(notParsed()){
        delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseNot(){
    #ifdef debug
    errorFunc("parseNot: "+to_string(_position));
    #endif
    expressions* e;
    char c = lookAhead();
    string s;
    try{
        if(c=='N'){
            s.push_back(c);
            _position++;
            s.push_back(_input[_position]);
            _position++;
            s.push_back(_input[_position]);
            if(s=="NOT" && _input[_position+1]==' '){
                _position++;
                return new notOperator(s, parseRelation());
            }else{
                _position=_position-2;
                e = parseRelation();
            }
            
        }else{
            e = parseRelation();
            return e;
        }
    }catch(wrongOperator()){
        delete e;
        *_parserPosition = _position;
        throw wrongOperator();
    }catch(wrongType()){
        delete e;
        *_parserPosition = _position;
        throw wrongType();
    }catch(notParsed()){
        delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
}

expressions* parser::parseRelation(){
    #ifdef debug
    errorFunc("parseRelation: "+to_string(_position));
    #endif
    expressions* e;
    char c;
    string s="";
    try{
        e = parseConcatenation();
        c = lookAhead();
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
    }catch(wrongOperator()){
        delete e;
        *_parserPosition = _position;
        throw wrongOperator();
    }catch(wrongType()){
        delete e;
        *_parserPosition = _position;
        throw wrongType();
    }catch(notParsed()){
        delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseConcatenation(){
    #ifdef debug
    errorFunc("parseConcatenation: "+to_string(_position));
    #endif
    expressions* e;
    char c;
    try{
        e = parseRange();
        c = lookAhead();
        while(c =='&'){
            _position++;
            e = new concatenationOperator(c, e, parseRange());
            c = lookAhead();
        }
    }catch(wrongType()){
        delete e;
        *_parserPosition = _position;
        throw wrongType();
    }catch(notParsed()){
        delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseRange(){
    #ifdef debug
    errorFunc("parseRange: "+to_string(_position));
    #endif
    uint8_t position = _position;
    expressions* e;
    char c;

    try{
        e = parseSum();
        c = lookAhead();
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
                    wrongStringRange();
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
    }catch(wrongRange()){
        delete e;
        *_parserPosition = _position;
        throw wrongRange();
    }catch(wrongStringRange()){
        delete e;
        *_parserPosition = _position;
        throw wrongStringRange();
    }catch(notParsed()){
        cout << "nieparsowalny "+to_string(_position);
        //delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseSum(){
    #ifdef debug
    errorFunc("parseSum: "+to_string(_position));
    #endif
    expressions* e;

    try{
        e = parseMult();
        char c = lookAhead();
        while(c == '+' || c == '-'){
            _position++;
            e = new binaryOperator(c, e, parseMult());
            c = lookAhead();
        }
    }catch(wrongOperator){
        delete e;
        *_parserPosition = _position;
        throw wrongOperator();
    }catch(wrongType){
        delete e;
        *_parserPosition = _position;
        throw wrongType();
    }catch(variableNotFound){
        delete e;
        *_parserPosition = _position;
        throw variableNotFound();
    }catch(notParsed){
        delete e;
        *_parserPosition = _position;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseMult(){
    #ifdef debug
    errorFunc("parseMult: "+to_string(_position));
    #endif
    expressions* e;

    try{
        e = parseTerm();
        char c = lookAhead();

        while(c == '*' || c == '/' || c == '^'){
            _position++;
            e = new binaryOperator(c, e, parseTerm());
            c = lookAhead();
        }
    }catch(wrongOperator()){
        delete e;
        *_parserPosition = _position;
        throw wrongOperator();
    }catch(wrongType()){
        delete e;
        *_parserPosition = _position;
        throw wrongType();
    }catch(notParsed){
        cout << "nieparsowalny "+to_string(_position);
        delete e;
        *_parserPosition = _position;
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
                    throw notParsed();
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
    char c = _input[_position];
    while(isalnum(c) || c == '%' || c == '$'){
        s.push_back(_input[_position]);
        _position++;
        c = _input[_position];
    }
    try{
        #if debug
        errorFunc("variable name: "+s);
        #endif
        return new variable(s);
    }catch(variableNotFound){
        *_parserPosition = _position;
        throw variableNotFound();
    }catch(variableNameAbsence){
        *_parserPosition = _position;
        throw variableNameAbsence();
    }
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