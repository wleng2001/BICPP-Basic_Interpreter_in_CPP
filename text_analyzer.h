#ifndef text_analyzer_h
#define text_analyzer_h

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

#include "tablesAndOtherConst.h"

//#define debug true

using namespace std;

class text_analyzer{
  private:

  int _position = 0;
  void (*_errorFunc)(string text);

  bool special_char_in_correct_place(string &data, int position);
  bool correctBracketQuantity(uint8_t *bracketQuantity, char c);

  public:

    bool *error;

    void addErrorFuntion(void (*errorFunction)(string text));
    
    string rewrite_in_range(uint8_t start, uint8_t end, string data) {
      string new_data = "";
      if (end < 0) {
        return "";
      }
      for (uint8_t i = start; i <= end; i = i + 1) {
        new_data += data[i];
      }
      return new_data;
    }

    void delete_spaces(string &data) {
      uint8_t i = 0;
      char analize_char = data[0];
      string new_data = "";
      while (analize_char == ' ') {
        analize_char = data[++i];
      }

      for (uint8_t j = i; j < data.length(); j++) {
        new_data += data[j];
      }
      data = new_data;
    }

    string rm_comment(string data) {
      if(data.find("REM")==string::npos){
        return data;
      }
      string new_data = rewrite_in_range(0, data.find("REM")-1, data);
      return new_data;
    }

    string trim(string data){
      int data_len = data.length();
      char sign=data[data_len-1];
      while(sign==' '){
        data_len--;
        sign=data[data_len-1];
      }
      return rewrite_in_range(0, data_len-1, data);
    }

    void up_case(string &data){
      string new_data="";
      bool start_quotation=0;
      int data_len = data.length();
      for(uint8_t i = 0; i<data_len; i++){
        if(data[i]=='"' || data[i] == '\''){
          start_quotation=!start_quotation;
        }
        if(start_quotation==0){
          new_data+=toupper(data[i]);
        }else{
          new_data+=data[i];
        }
      }
      data = new_data;
    }

    void delete_useless_spaces(string &data);

    string one_word_ret(string data);

    string reform_input(string &data);
};

#endif