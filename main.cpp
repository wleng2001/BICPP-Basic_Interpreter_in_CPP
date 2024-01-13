#include <iostream>
#include "basic.h"

using namespace std;

void printFunction(string text){
    cout << text;
}

void errorFunction(string text){
    cout << "\033[31m" << text << "\033[0m" << endl;
}

string inputFunction(){
    string text;
    getline(cin, text);
    return text;
}

basic bas(printFunction, errorFunction, inputFunction);
#ifdef _WIN32
bool interrupt(){
    char var;
    if(GetKeyState(VK_ESCAPE) & 0x8000){
        return 1;
    }else{
        return 0;
    }
}
#endif

int main(){
    #ifdef _WIN32
    system("cls");
    bas.addInterruptFunc(interrupt);
    #else
    system("clear");
    #endif
    cout << "Basic interpreter example implementation" << endl;
    cout << "Version: "<< _basicVersion << endl;
    
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
