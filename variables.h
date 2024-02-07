#ifndef variables_h
#define variables_h

#include <iostream>
#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#include <cmath>
#ifdef _WIN32
#include <windows.h> //unistd.h dla linux
#include <conio.h> //wprowadzanie danych bez entera
#else
#include <unistd.h>
#endif
#include <cstdlib> //biblioteka, aby działały komendy sytemowe np: system("cls");
#include <time.h>
#include <fstream> //biblioteka do obsługi plików
#include <algorithm>
#include <math.h>
#include <map>
#include <vector>
#include "errorClasses.h"
#include "config.h"

#define BOOL 0
#define INT 1
#define FLOAT 2
#define STRING 3

#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15


using namespace std;

struct variableValue{
    char type = 'N'; // i - int, n - numeric, s - string, N - NULL
    int valueI;
    float valueN;
    #if arduino 
    String valueS;
    #else
    string valueS;
    #endif

};

class variables{
    private:

    void (*errorFunc)(string text);
    bool *error;

    bool isHexLetter(char c);

    struct variableParamI{
        bool isArray = false;
        uint8_t rows = 0;
        uint8_t columns = 0;
        vector <int> value;
    };

    struct variableParamN{
        bool isArray = false;
        uint8_t rows = 0;
        uint8_t columns = 0;
        vector <float> value;
    };

    struct variableParamS{
        bool isArray = false;
        uint8_t rows = 0;
        uint8_t columns = 0;
        #if arduino
        vector <String> value;
        #else
        vector <string> value;
        #endif
    };

    #if arduino
    typedef std::map < String, variableParamI > variablesListI;
    typedef std::map < String, variableParamN > variablesListN;
    typedef std::map < String, variableParamS > variablesListS;
    #else
    typedef std::map < string, variableParamI > variablesListI;
    typedef std::map < string, variableParamN > variablesListN;
    typedef std::map < string, variableParamS > variablesListS;
    #endif

    variablesListI _VLI;
    variablesListN _VLN;
    variablesListS _VLS;

    public:
    #if arduino
    void addErrorFunction(void (*errorFunction)(String text));
    void addErrorVariable(bool *error);
    String convertScienceToDecimal(String data);
    bool isInt(String &data, uint8_t length);
    bool isNum(String &data, uint8_t length);
    String convertHexToDecimal(String data);
    void clearMemory();
    void addVariable(String &variableName, String &value);
    bool readVariable(String *variableName, variableValue *var);
    #else
    void addErrorFunction(void (*errorFunction)(string text));
    void addErrorVariable(bool *error);
    string convertScienceToDecimal(string data);
    bool isInt(string &data, uint8_t length);
    bool isNum(string &data, uint8_t length);
    string convertHexToDecimal(string data);
    void clearMemory();
    void addVariable(string &variableName, string &value);
    bool readVariable(string *variableName, variableValue *var);
    #endif
};

#endif