#ifndef variables_h
#define variables_h

#include <iostream>
#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#include <cmath>
#include <windows.h> //unistd.h dla linux
#include <cstdlib> //biblioteka, aby działały komendy sytemowe np: system("cls");
#include <time.h>
#include <conio.h> //wprowadzanie danych bez entera
#include <fstream> //biblioteka do obsługi plików
#include <algorithm>
#include <math.h>

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

class variables{
    public:
    void defVariable(string &variables_name, string &variables_value, int quantity_of_variables){
        int x;
    }

    string convertHexToDecimal(string data);

    string returnValue(){//last char is type of variable value allways in decimal format
        int y;
    }
};

#endif