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
    #if debug 
    errorFunc("parserStatements: "+to_string(_position));
    #endif
    expressions *e = nullptr;
    char c = lookAhead();
    string statement;
    try{
        while(isalpha(c)){
            statement.push_back(c);
            _position++;
            c = _input[_position];
        }
        if(c == ' ' || c==0){
            if(parseRem(statement)==nullptr){
                return new constantS("");
            }
            e = parseLet(statement);

            if(e==nullptr){
                _position = 0;
                return parseFunction();
            }
        }else{
            _position = 0;
            return parseFunction();
        }
    }catch(wrongType){
        delete e;
        throw wrongType();
        return e;
    }catch(wrongVariableName){
        delete e;
        throw;
    }catch(std::out_of_range){
        delete e;
        _position = 0;
        throw notParsed();
    }catch(notParsed){
        #if debug
        errorFunc("nieparsowalny statement");
        #endif
        delete e;
        throw;
    }
    return e;
}

expressions* parser::parseRem(string statement){
    expressions *e;
    if(statement == "REM"){
        while(lookAhead()!=0){
            _position++;
        }
        e = nullptr;
        return e;
    }
    return e;
}

expressions* parser::parseLet(string statement){
    expressions *e = nullptr;
    if(statement == "LET"){
        #if debug
        errorFunc("parseLet: "+to_string(_position));
        #endif
        char c = lookAhead();
        string s;
        if(isdigit(c)){
            throw wrongVariableName();
        }
        while(c!='='){
            s.push_back(c);
            _position++;
            c = lookAhead();
        }
        #if debug
        errorFunc("varName: "+s);
        #endif
        _position++;
        try{
            if(s!=""){
                e = new letStatement(s, parseLogical());
                return e;
            }else{
                throw variableNameAbsence();
                return e;
            }
                    }catch(wrongType()){
            delete e;
            throw wrongType();
            return e;
        }catch(notParsed()){
            delete e;
            throw notParsed();
        }catch(...){
            delete e;
            throw;
        }
    }else{
        delete e;
        return e;
    }

}

expressions* parser::parseInput(string statement){
    expressions* e = nullptr;
    if(statement=="INPUT"){

    }
}

expressions* parser::parseFunction(){
    #if debug
    errorFunc("parseFunction: "+to_string(_position));
    #endif
    return parseLogical();
}

expressions* parser::parseLogical(){
    #if debug
    errorFunc("parseLogical: "+to_string(_position));
    #endif
    expressions *e = nullptr;
    char c;
    string s = "";
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
                if(_input[_position]==' ' || _input[_position+1]=='(' && (s == "AND" || s == "XOR" || s == "IMP" || s =="EQV")){
                    e = new logicalOperator(s, e, parseNot());
                }
                s="";
            }
            c=lookAhead();

        }
    }catch(wrongOperator()){
        delete e;
        throw wrongOperator();
    }catch(wrongType()){
        delete e;
        throw wrongType();
    }catch(notParsed()){
        #if debug
        errorFunc("nieparsowalny logical");
        #endif
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseNot(){
    #if debug
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
            if(s=="NOT" && _input[_position+1]==' ' || _input[_position+1]=='('){
                _position++;
                return new notOperator(s, parseRelation());
            }else{
                _position=_position-3;
                e = parseRelation();
            }
            
        }else{
            e = parseRelation();
            return e;
        }
    }catch(wrongOperator()){
        delete e;
        throw wrongOperator();
    }catch(wrongType()){
        delete e;
        throw wrongType();
    }catch(notParsed()){
        #if debug
        errorFunc("nieparsowalny not");
        #endif
        delete e;
        throw notParsed();
    }
}

expressions* parser::parseRelation(){
    #if debug
    errorFunc("parseRelation: "+to_string(_position));
    #endif
    expressions* e;
    char c;
    string s="";
    try{
        e = parseConcatenation();
        c = lookAhead();
        while(c=='>' || c=='<' || c=='='){
            s+=c;
            _position++;
            c = _input[_position];
            if(c=='='||c=='<'||c=='>'){
                s+=c;
                _position++;
                e = new relationOperator(s, e, parseConcatenation());
            }else{
                e = new relationOperator(s, e, parseConcatenation());
            }
            c = lookAhead();
            s="";
        }
    }catch(wrongOperator()){
        delete e;
        throw;
    }catch(wrongType()){
        delete e;
        throw wrongType();
    }catch(notParsed()){
        #if debug
        errorFunc("nieparsowalny relation");
        #endif
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseConcatenation(){
    #if debug
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
        throw wrongType();
    }catch(notParsed()){
        #if debug
        errorFunc("nieparsowalny concatenation");
        #endif
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseRange(){
    #if debug
    errorFunc("parseRange: "+to_string(_position));
    #endif
    uint8_t position = _position;
    expressions* e;
    char c;
    if(_input.rfind('[') != string::npos){
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
                        throw wrongStringRange();
                    }
                }else{
                    delete e;
                    delete startRange;
                    throw wrongStringRange();
                }
                return e;
            }
        }catch(wrongRange()){
            delete e;
            throw wrongRange();
        }catch(wrongStringRange()){
            delete e;
            throw wrongStringRange();
        }catch(notParsed()){
            #if debug
            errorFunc("nieparsowalny range");
            #endif
            delete e;
            throw notParsed();
        }
    }
    return parseSum();
}

expressions* parser::parseSum(){
    #if debug
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
        throw wrongOperator();
    }catch(wrongType){
        delete e;
        throw wrongType();
    }catch(notParsed()){
        #if debug
        errorFunc("nieparsowalny sum");
        #endif
        delete e;
        throw notParsed();
    }
    return e;
}

expressions* parser::parseMult(){
    #if debug
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
    }catch(wrongOperator){
        delete e;
        throw wrongOperator();
    }catch(wrongType){
        delete e;
        throw wrongType();
    }catch(notParsed){
        #if debug
        errorFunc("nieparsowalny mult");
        #endif
        throw notParsed();
    }
    return e;
}

expressions* parser::parseTerm(){
    #if debug
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
        throw notParsed();
}

expressions* parser::parseConstant(){
    string n = "";
    bool isDecimalSeparator = false;
    bool isNumber = false;
    #if debug
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
    float value;
    if(isNumber == true){
        if(isDecimalSeparator==false)
            return new constant(stoi(n));
        else
            try{
                value = stof(n);
            }catch(std::invalid_argument){
                throw notNumber();
            }
            return new constantN(value);
    }else{
        return new constantS(n);
    }
}

expressions* parser::parseLogicalVariable(){
    string s = "";
    #if debug
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
    #if debug
    errorFunc("parseVariable: "+to_string(_position));
    #endif
    expressions *e;
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
        e = new variable(s);
        return e;
    }catch(variableNotFound){
        delete e;
        throw variableNotFound();
    }catch(variableNameAbsence){
        delete e;
        throw variableNameAbsence();
    }
}

expressions* parser::parseParen(){
    _position++;
    #if debug
    errorFunc("parseParen: "+to_string(_position));
    #endif
    expressions* e = parseLogical();
    if(lookAhead() == ')'){
        _position++;
        return e;
    }else{
        delete e;
        throw notParsed();
    }
}