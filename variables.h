#ifndef variables_h
#define variables_h

#if arduino
#include <Vector.h>
#include <StandardCplusplus.h>
#include <Arduino.h>
#else
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
#include <map>
#include <vector>
#endif
#include <math.h>
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

    #if arduino
    void (*errorFunc)(String text);
    #else
    void (*errorFunc)(string text);
    #endif
    bool *error;

    bool isHexLetter(char c);

    struct variableParamI{
        bool isArray = false;
        uint8_t rows = 0;
        uint8_t columns = 0;
        #if arduino
        Vector <int> value;
        #else
        vector <int> value;
        #endif
    };

    struct variableParamN{
        bool isArray = false;
        uint8_t rows = 0;
        uint8_t columns = 0;
        #if arduino
        Vector <float> value;
        #else
        vector <float> value;
        #endif
    };

    struct variableParamS{
        bool isArray = false;
        uint8_t rows = 0;
        uint8_t columns = 0;
        #if arduino
        Vector <String> value;
        #else
        vector <string> value;
        #endif
    };

    #if arduino
    std::map < String, variableParamI > _VLI;
    std::map < String, variableParamN > _VLN;
    std::map < String, variableParamS > _VLS;
    #else
    typedef std::map < string, variableParamI > variablesListI;
    typedef std::map < string, variableParamN > variablesListN;
    typedef std::map < string, variableParamS > variablesListS;
    variablesListI _VLI;
    variablesListN _VLN;
    variablesListS _VLS;
    #endif

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
