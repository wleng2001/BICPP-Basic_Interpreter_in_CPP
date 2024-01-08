#include <iostream>
#include "variables.h"
#include "variables.cpp"

void errorFunction(string text){
    cout << "\033[31m" << text << "\033[0m" << endl;
}

bool error;
string var_name = "";
string value = "";

variableValue val;

variables var;

using namespace std;

int main(){
    var.addErrorFunction(errorFunction);
    var.addErrorVariable(&error);
    while(true){
        if(error==1){
            break;
        }
        cout << "Nazwa zmiennej: ";
        cin >> var_name;
        if(var_name=="0")
            break;
        cout << "Wartosc zmiennej: ";
        cin >> value;
        var.addVariable(var_name, value);
    }
    while(true){
        cout << "Nazwa zmiennej do odczytania: ";
        cin >> var_name;
        if(var.readVariable(&var_name, &val))
            cout << val.type << '\t' << val.valueI << '\t' << val.valueN << '\t' << val.valueS << endl;
    }
    return 0;
}