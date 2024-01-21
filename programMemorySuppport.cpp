#include "programMemorySuppport.h"

using namespace std;

bool programMemorySupport::checkMathematicSymbol(char input){
    for(uint8_t i = 0; i<sizeof(mathematicChar)/sizeof(mathematicChar[0]); i++){
        if(mathematicChar[i]==input){
            return true;
        }
    }
    return false;
}

int programMemorySupport::takeDigits(string &input){
  string number = "";
  while(isdigit(input[_position])){
    number+=input[_position];
    _position++;
  }

  #if debug
  cout << "LineNumber: " << number << endl;
  #endif

  if(number == ""){
    return 0;
  }
  return stoi(number);
}

void programMemorySupport::skipWhiteSpace(string &input){
    while(isspace(input[_position])){
        _position++;
    }
}

unsigned int programMemorySupport::quantityOfSpecificChar(string &input, char c){
    unsigned int length = input.length();
    unsigned int quantityOfFindElements = 0;
    for(auto i = 0; i < input.length(); i++){
        if(input[i] == c){
            quantityOfFindElements++;
        }
    }

    #if debug
    cout << "Quantity of " << c << ": " << quantityOfFindElements << endl;
    #endif 

    return quantityOfFindElements;
}

void programMemorySupport::memoryClear(){
    _pM.clear();
}

bool programMemorySupport::isToExecute(string &input){
    input.push_back(0);
    if(isdigit(input[_position])){
        _position++;
        while(isdigit(input[_position])){
            _position++;
        }

        int priorPosition = _position;
        #if debug
        cout << "prior: " << priorPosition << endl;
        cout << "post: "<< _position << endl;
        #endif

        skipWhiteSpace(input);
        if(_position == priorPosition && checkMathematicSymbol(input[_position]) || input[_position]==0){
            _position = 0;

            #if debug
            cout << "prior1: " << priorPosition << endl;
            cout << "post: "<< _position << endl;
            #endif

            return true;
        }else{
            #if debug
            cout << "prior3: " << priorPosition << endl;
            cout << "post: "<< _position << endl;
            #endif
            return false;
        }
    }else
        return true;
}

bool programMemorySupport::checkAndSave(string &input){
    unsigned int lineNumber = 0;
    unsigned int lineQuantity = 1;
    if(!isToExecute(input)){
        _position = 0;
        lineQuantity = quantityOfSpecificChar(input, '\n')+1;
        for(auto i = 0; i< lineQuantity; i++){
            lineNumber = takeDigits(input);
            if(lineNumber<=0){
                _errorFunc("Error: line number must be bigger than 0: "+input);
                return 0;
            }
            input.erase(0, _position);

            #if debug
            cout << "input1: " << input << endl;
            #endif

            _position = 0;
            skipWhiteSpace(input);
            if(_position>0){
                input.erase(0, _position);
                _position = 0;
            }

            #if debug
            cout << "input2: " << input << endl;
            #endif

            if(lineQuantity == 1)
                _pM[lineNumber] = input;
            else
                _pM[lineNumber] = input.erase(0, input.find('\n'));

            #if debug
            cout << "Line value: " << _pM[lineNumber] << endl;
            #endif
            
            if(lineNumber > maxLine){
                maxLine = lineNumber;
            }
            _position = 0;
        }
        return 0;
    }
    return 1;
}