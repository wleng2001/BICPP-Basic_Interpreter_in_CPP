#include "variables.h"

string variables::convertHexToDecimal(string data){
    if(data[0] == '0'){
        int length=data.length();
        int value = 0;
        if(data[length-1]=='H'){
            data = data.substr(1, length-2);
            length = data.length();
            for(int i = length-1; i>=0 ; i--){
                char i_value = data[i];
                switch(i_value){
                    case 'A':
                        value+=A*pow(16, length-1-i);
                        break;
                    case 'B':
                        value+=B*pow(16, length-1-i);
                        break;
                    case 'C':
                        value+=C*pow(16, length-1-i);
                        break;
                    case 'D':
                        value+=D*pow(16, length-1-i);
                        break;
                    case 'E':
                        value+=E*pow(16, length-1-i);
                        break;
                    case 'F':
                        value+=F*pow(16, length-1-i);
                        break;
                    default:
                        value+=(data[i]-48)*pow(16, length-1-i);
                        break;
                }
            }
            data = to_string(value);
            return data;
        }
    }else{
        return data;
    }
}