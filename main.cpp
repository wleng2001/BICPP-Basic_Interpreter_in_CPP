#include <iostream>
/*#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#include <cmath>
#include <windows.h> //unistd.h dla linux
#include <cstdlib> //biblioteka, aby działały komendy sytemowe np: system("cls");
#include <time.h>
#include <conio.h> //wprowadzanie danych bez entera
#include <fstream> //biblioteka do obsługi plików
#include <algorithm>*/
#include "basic.h"

using namespace std;

string text = "jakis tekst \n i inny tekst teraz \n teraz jeszcze inny \n";

void printFunction(string text){
    cout << text;
}

void errorFunction(string text){
    cout << text << endl;
}

string inputFunction(){
    string text;
    getline(cin, text);
    return text;
}

basic bas(printFunction, errorFunction, inputFunction);

bool interrupt(){
    char var;
    if(GetKeyState(VK_ESCAPE) & 0x8000){
        return 1;
    }else{
        return 0;
    }
}

int main(){
    system("cls");
    bas.addInterruptFunc(interrupt);
    cout << "Basic interpreter example program" << endl;
    
    while(true){
        string text;
        cout << ">>>";
        getline(cin, text);
        string output = bas.run(&text);
        if(output != "")
            cout << output << endl;
    }
    
    return 0;
}
