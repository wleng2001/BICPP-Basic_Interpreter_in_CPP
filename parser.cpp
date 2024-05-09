#include "parser.h"

const char EOS = 0;

#if arduino
String parser::returnString(expressions *e){
#else
string parser::returnString(expressions *e){
#endif
    variableValue vV = e->eval(_vM);
    switch(vV.type){
        case('s'):
            return vV.valueS;
        case('i'):
            #if arduino
            return String(vV.valueI);
            #else
            return to_string(vV.valueI);
            #endif
        case('n'):
            #if arduino
            return String(vV.valueN);
            #else
            return to_string(vV.valueN);
            #endif
        default:
            throw wrongType();
    }
}

void parser::setProgramLine(unsigned int programLine){
    if(programLine>0){
        *_programLine = programLine;
    }
}

bool parser::valueBiggerThan0(variableValue *vV){
    switch(vV->type){
        case('i'):
            if(vV->valueI>0)
                return 1;
            else
                return 0;
            break;
        case('n'):
            if(vV->valueN>0)
                return 1;
            else
                return 0;
            break;
        case('s'):
            if(vV->valueS.length()>0)
                return 1;
            else
                return 0;
            break;
        default:
            throw wrongType();
    }
    
}

#if arduino
parser::parser(String &input, void (*errorFunction)(String input), String (*inputFunction)(), void printFunction(String *input), variables *variableMemory, programMemorySupport *pMS, unsigned int *programLineIterator) : _input(input), _position(0), errorFunc(errorFunction), _inputFunc(inputFunction), _printFunc(printFunction), _vM(variableMemory), _pMS(pMS), _programLine(programLineIterator){
    input += EOS; //umieszcza strażnika na końcu
#else
parser::parser(string &input, void (*errorFunction)(string input), string (*inputFunction)(), void printFunction(string *input), variables *variableMemory, programMemorySupport *pMS, unsigned int *programLineIterator) : _input(input), _position(0), errorFunc(errorFunction), _inputFunc(inputFunction), _printFunc(printFunction), _vM(variableMemory), _pMS(pMS), _programLine(programLineIterator){
    input.push_back(EOS); //umieszcza strażnika na końcu
#endif
    
    expErrorFunc = errorFunction;
}

#if arduino
parser::parser( void (*errorFunction)(String input), String (*inputFunction)(), void printFunction(String *input), variables *variableMemory, programMemorySupport *pMS) :  _position(0), errorFunc(errorFunction), _inputFunc(inputFunction), _printFunc(printFunction), _vM(variableMemory), _pMS(pMS){
#else
parser::parser( void (*errorFunction)(string input), string (*inputFunction)(), void printFunction(string *input), variables *variableMemory, programMemorySupport *pMS) :  _position(0), errorFunc(errorFunction), _inputFunc(inputFunction), _printFunc(printFunction), _vM(variableMemory), _pMS(pMS){
#endif
    expErrorFunc = errorFunction;
}

#if arduino
void parser::addInput(String &input){
    input += EOS;
#else
void parser::addInput(string &input){
    input.push_back(EOS);
#endif
    _input = input;
    _position = 0;
}

void parser::addProgramLine(unsigned int *programLine){
    _programLine = programLine;
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

//parseStatementStart-------------------------------------------------------

expressions* parser::parseStatements(bool execute){
    #if debug 
    errorFunc("parserStatements: "+to_string(_position));
    #endif
    expressions *e = nullptr;
    char c = lookAhead();
    #if arduino
    String statement;
    #else
    string statement;
    #endif
    try{
        while(isalpha(c)){
            #if arduino
            statement+=c;
            #else
            statement.push_back(c);
            #endif
            _position++;
            c = _input[_position];
        }
        if(c == ' ' || c==0){
            bool parsed;
            parsed = parseRun(statement, parsed);
            if(parseRem(statement, parsed)==true){
                return new constantS("");
            }
            parsed = parseLet(statement, parsed, execute);
            parsed = parseInput(statement, parsed, execute);
            parsed = parsePrint(statement, parsed, execute);
            parsed = parseClear(statement, parsed, execute);
            parsed = parseGoto(statement, parsed, execute);
            parsed = parseIf(statement, parsed, execute);
            if(parsed==false){
                _position = 0;
                return parseFunction();
            }else{
                return new constantS("");
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
    #if arduino

    #else
    }catch(std::out_of_range){
        delete e;
        _position = 0;
        throw notParsed();
    #endif
    }catch(tooManyArg){
        delete e;
        throw;
    }catch(notParsed){
        #if debug
        errorFunc("nieparsowalny statement");
        #endif
        delete e;
        throw;
    }
    return e;
}

#if arduino
bool parser::parseRun(String statement, bool parsed, bool execute){
#else
bool parser::parseRun(string statement, bool parsed, bool execute){
#endif
    if(statement == "RUN"){
        char c = lookAhead();
        if(c!=0){
            throw tooManyArg();
        }else{
            setProgramLine(1);
            return true;
        }
    }
    return parsed;
}

#if arduino
bool parser::parseRem(String statement, bool parsed, bool execute){
#else
bool parser::parseRem(string statement, bool parsed, bool execute){
#endif
    if(statement == "REM"){
        while(lookAhead()!=0){
            _position++;
        }
        return true;
    }
    return parsed;
}

#if arduino
bool parser::parseLet(String statement, bool parsed, bool execute){
#else
bool parser::parseLet(string statement, bool parsed, bool execute){
#endif
    if(statement == "LET"){
        expressions *e;
        #if debug
        errorFunc("parseLet: "+to_string(_position));
        #endif
        char c = lookAhead();
        #if arduino
        String s;
        #else
        string s;
        #endif
        if(isdigit(c)){
            throw wrongVariableName();
        }
        while(c!='='){
            #if arduino
            s += c;
            #else
            s.push_back(c);
            #endif
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
                e->eval(_vM);
            }else{
                throw variableNameAbsence();
            }
        }catch(wrongType()){
            delete e;
            throw wrongType();
        }catch(notParsed()){
            delete e;
            throw notParsed();
        #if arduino

        #else
        }catch(std::bad_alloc){
            delete e;
            throw;
        #endif
        }
        delete e;
        return true;
    }else{
        return parsed;
    }

}

#if arduino
bool parser::parseInput(String statement, bool parsed, bool execute){
#else
bool parser::parseInput(string statement, bool parsed, bool execute){
#endif
    if(statement=="INPUT"){
        expressions *internalE;
        #if debug
        errorFunc("parseInput");
        #endif
        char c = lookAhead();
        if(c=='"'){
            try{
                _position++;
                internalE = parseLiteral();
            }catch(...){
                delete internalE;
                throw notParsed();
            }
            variableValue prompt = internalE->eval(_vM);
            _printFunc(&prompt.valueS);
        }
        #if arduino
        vector <String> variableList;
        #else
        vector <string> variableList;
        #endif

        if(lookAhead()==0){
            throw notParsed();
        }
        do{
            c = lookAhead();
            #if arduino
            String variableName = String(c);
            #else
            string variableName = string(1,c);
            #endif
            _position++;
            internalE = parseVariable(variableName, true);
            variableName = internalE->eval(_vM).valueS;
            variableList.push_back(variableName);
        }while(c==',');
        #if arduino
        String value = _inputFunc();
        #else
        string value = _inputFunc();
        #endif
        int position = 0;
        for(uint8_t i = 0; i<variableList.size(); i++){
            #if arduino
            String oneVariableValue;
            #else
            string oneVariableValue;
            #endif
            c = value[position];
            while(c!=',' && c!=0){
                #if arduino
                oneVariableValue += c;
                #else
                oneVariableValue.push_back(c);
                #endif
                position++;
                c = value[position];
            }
            try{
                _vM->addVariable(variableList[i],oneVariableValue);
            #if arduino

            #else
            }catch(std::bad_alloc){
                delete internalE;
                throw;
            #endif
            }catch(...){
                delete internalE;
                _input = value;
                _position = position;
                throw wrongType();
            }

        }
        delete internalE;
        return true;
    }else{
        return parsed;
    }
}

#if arduino
bool parser::parsePrint(String statement, bool parsed, bool execute){
#else
bool parser::parsePrint(string statement, bool parsed, bool execute){
#endif
    if(statement == "PRINT"){
        #if debug
        errorFunc("parsePrint");
        #endif
        #if arduino
        String output = "";
        #else
        string output = "";
        #endif
        char c;
        do{
            _position++;
            expressions *e = parseFunction();
            try{
                output += returnString(e);
                delete e;
            }catch(wrongType){
                delete e;
                throw;
            }
            c = lookAhead();
        }while(c==',');
        output+='\n';
        if(execute)
            _printFunc(&output);
        return true;
    }
    return parsed;
}

#if arduino
bool parser::parseClear(String statement, bool parsed, bool execute){
#else
bool parser::parseClear(string statement, bool parsed, bool execute){
#endif
    if(statement == "CLEAR"){
        char c = lookAhead();
        if(c==0){
            if(execute){
                _vM->clearMemory();
                _pMS->memoryClear();
            }
            return true;
        }else{
            throw tooManyArg();
        }
    }
    return parsed;
}

#if arduino
bool parser::parseGoto(String statement, bool parsed, bool execute){
#else
bool parser::parseGoto(string statement, bool parsed, bool execute){
#endif
    if(statement == "GOTO"){
        #if debug
        errorFunc("parseGoto");
        #endif
        expressions *e;
        variableValue vV;
        try{
            lookAhead();
            e = parseConstant();
            vV = e->eval(_vM);
        }catch(...){
            delete e;
            throw;
        }
        
        if(vV.type!='i'){
            throw wrongType();
        }
        if(vV.valueI>10000){
            throw wrongRange();
        }
        setProgramLine(vV.valueI-1);
        delete e;   
        return true;
    }
    return parsed;
}

#if arduino
bool parser::parseIf(String statement, bool parsed, bool execute){
#else
bool parser::parseIf(string statement, bool parsed, bool execute){
#endif
    if(statement == "IF"){
        #if debug
        errorFunc("parseIf");
        #endif
        expressions *e;
        variableValue vV;
        try{
            lookAhead();
            e = parseFunction();
            vV = e->eval(_vM);

            #if arduino
            String t;
            #else
            string t;
            #endif
            while(!isspace(_input[_position]) && _position < _input.length()){
                t+=_input[_position];
                _position++;
            }
            if(t=="THEN"){
            }else{
                throw wrongSyntax();
            }

            bool b = valueBiggerThan0(&vV);
            #if debug
            errorFunc("Term value: "+to_string(b));
            #endif
            if(b){
                e = parseStatements();
                e->eval(_vM);
                _position = _input.length();
            }else{
                /*
                t= "";
                while(!isspace(_input[_position]) && _position < _input.length()){
                    t+=_input[_position];
                    _position++;
                }
                if(t=="ELSE"){
                    e = parseStatements();
                    e->eval(_vM);
                }*/
                _position = _input.length()-1;
                
            }
        }catch(...){
            
            throw;
            
        }
        delete e;
        return true;
    }else{
        return parsed;
    }
}

//parseStatementEnd---------------------------------------------------------

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
    #if arduino
    String s = "";
    #else
    string s = "";
    #endif
    try{
        e = parseNot();
        c = lookAhead();
        while( c=='O' || c=='A' || c=='X' || c=='I' || c == 'E'){
            #if arduino
            s += c;
            #else
            s.push_back(c);
            #endif
            _position++;
            #if arduino
            s += _input[_position];
            #else
            s.push_back(_input[_position]);
            #endif
            if(_input[_position+1]==' '){
                if(s=="OR"){
                    _position++;
                    e = new logicalOperator(s, e, parseNot());
                    s="";
                }
            }else{
                _position++;
                #if arduino
                s += _input[_position];
                #else
                s.push_back(_input[_position]);
                #endif
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
    #if arduino
    String s;
    #else
    string s;
    #endif
    try{
        if(c=='N'){
            #if arduino
            s += c;
            _position++;
            s += _input[_position];
            _position++;
            s += _input[_position];
            #else
            s.push_back(c);
            _position++;
            s.push_back(_input[_position]);
            _position++;
            s.push_back(_input[_position]);
            #endif

            if(s=="NOT" && _input[_position+1]==' ' || _input[_position+1]=='('){
                _position++;
                return new notOperator(s, parseRelation());
            }else{
                _position=_position-2;
                e = parseRelation();
                return e;
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
    #if arduino
    String s="";
    #else
    string s="";
    #endif
    try{
        e = parseConcatenation();
        c = lookAhead();
        while(c=='>' || c=='<' || c=='=' || c=='!'){
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
    #if arduino
    size_t findPos = _input.indexOf('[');
    #else
    size_t findPos = _input.find('[');
    #endif
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
                    _position++;
                    return new substringOperation(e, startRange, endRange);
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
    return e;
}

expressions* parser::parseSum(){
    #if debug
    errorFunc("parseSum: "+to_string(_position));
    #endif
    expressions* e;

    try{
        char c = lookAhead();
        if(c=='-'){
            _position++;
            e = new binaryOperator('-', new constant(0), parseMult());
        }else{
            e = parseMult();
        }
        c = lookAhead();
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
    }else{
        #if debug
        errorFunc("nieparsowalny Term");
        #endif
        throw notParsed();
    }
}

expressions* parser::parseConstant(){
    #if arduino
    String n="";
    #else
    string n="";
    #endif
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
                    this->errorFunc("Error: to much pointer: "+_input);
                    throw notParsed();
                }
            }
            _position++;
        }
    }else{
        _position++;
        return parseLiteral();
    }
        
    float value;
    if(isNumber == true){
        if(isDecimalSeparator==false)
            #if arduino
            return new constant(n.toInt());
            #else
            return new constant(stoi(n));
            #endif
        else{
            #if arduino
            value = n.toFloat();
            #else
            try{
                value = stof(n);
            }catch(std::invalid_argument){
                throw notNumber();
            }
            #endif
        }   
            return new constantN(value);
    }

    return new constantS("");
}

expressions* parser::parseLiteral(){
    #if debug
    errorFunc("parseLiteral");
    #endif
    #if arduino
    String literal = "";
    #else
    string literal = "";
    #endif
    char c = lookAhead();
    bool endAnalyze = false;

    while(!endAnalyze){
        if(c==0)
            throw notParsed();
        switch(c){
            case('"'):
                if(_input[_position+1]=='"'){
                    literal+=c;
                    _position+=2;
                    break;
                }else{
                    _position++;
                    endAnalyze = true;
                    break;
                }
                break;

            case('\\'):
                switch(_input[_position+1]){
                    case('n'):
                        literal+='\n';
                        break;
                    case('t'):
                        literal+='\t';
                        break;
                }
                _position+=2;
                break;
                
            default:
                literal+=c;
                _position++;
        }
        c = _input[_position];
    }
    return new constantS(literal);
}

expressions* parser::parseLogicalVariable(){
    #if arduino
    String s="";
    #else
    string s="";
    #endif
    #if debug
    errorFunc("parserLogicalVariable: "+to_string(_position));
    #endif
    int position = _position;
    while(isalpha(_input[_position])){
        #if arduino
        s+=_input[_position];
        #else
        s.push_back(_input[_position]);
        #endif
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

#if arduino
expressions* parser::parseVariable(String &s, bool returnName){
#else
expressions* parser::parseVariable(string &s, bool returnName){
#endif
    #if debug
    errorFunc("parseVariable: "+to_string(_position));
    #endif
    expressions *e = nullptr;
    char c = _input[_position];
    while(isalnum(c) || c == '%' || c == '$'){
        #if arduino
        s += _input[_position];
        #else
        s.push_back(_input[_position]);
        #endif
        _position++;
        c = _input[_position];
    }
    try{
        #if debug
        errorFunc("variable name: "+s);
        #endif
        if(returnName==false){
            e = new variable(s);
        }else{
            e = new constantS(s);
        }
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