#ifndef config_h
#define config_h

#define debug false
#define arduino false//true not available yet
#define stringAvailable true //false not available yet

#if arduino
#include <ArduinoSTL.h>
#include <Arduino.h>
#else
#include <iostream>
#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#include <cmath>
#ifdef _WIN32
#include <windows.h> //unistd.h dla linux
#include <conio.h>   //wprowadzanie danych bez entera
#else
#include <unistd.h>
#endif
#include <cstdlib> //biblioteka, aby działały komendy sytemowe np: system("cls");
#include <time.h>
#include <fstream> //biblioteka do obsługi plików
#include <algorithm>
#include <vector>
#endif
#include <map>
#include <math.h>
#include "errorClasses.h"
#include "tablesAndOtherConst.h"

using namespace std;

#endif
