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

#if arduino
int programMemorySupport::takeDigits(String &input){
    String number = "";
#else
int programMemorySupport::takeDigits(string &input){
    string number = "";
#endif
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
  #if arduino
  return number.toInt();
  #else
  return stoi(number);
  #endif
}

#if arduino
void programMemorySupport::skipWhiteSpace(String &input){
#else
void programMemorySupport::skipWhiteSpace(string &input){
#endif
    while(isspace(input[_position])){
        _position++;
    }
}

#if arduino
unsigned int programMemorySupport::quantityOfSpecificChar(String &input, char c){
#else
unsigned int programMemorySupport::quantityOfSpecificChar(string &input, char c){
#endif
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

#if arduino
bool programMemorySupport::isToExecute(String &input){
    input+=0;
#else
bool programMemorySupport::isToExecute(string &input){
    input.push_back(0);
#endif
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

#if arduino
bool programMemorySupport::checkAndSave(String &input){
#else
bool programMemorySupport::checkAndSave(string &input){
#endif
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
            #if arduino
            input.remove(0, _position);
            #else
            input.erase(0, _position);
            #endif

            #if debug
            cout << "input1: " << input << endl;
            #endif

            _position = 0;
            skipWhiteSpace(input);
            if(_position>0){
                #if arduino
                input.remove(0, _position);
                #else
                input.erase(0, _position);
                #endif
                _position = 0;
            }

            #if debug
            cout << "input2: " << input << endl;
            #endif

            if(lineQuantity == 1)
                _pM[lineNumber] = input;
            else
                #if arduino
                _pM[lineNumber] = input.substring(0, input.indexOf('\n'));
                #else
                _pM[lineNumber] = input.erase(0, input.find('\n'));
                #endif

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