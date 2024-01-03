#include "parser.h"

const char EOS = 0;

parser::parser(string input) : _input(input), _position(0){
    input.push_back(EOS); //umieszcza strażnika na końcu
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
    if(isdigit(c)){
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
    while(isdigit(_input[_position]) || _input[_position]=='.'){
        n+= _input[_position];
        _position++;
    }
    return new constant(stof(n));
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