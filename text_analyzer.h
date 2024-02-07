#ifndef text_analyzer_h
#define text_analyzer_h

#include "config.h"
#include "tablesAndOtherConst.h"

#if arduino
#include <math.h>

#else
#include <iostream>
#include <stdio.h> //aby działał getchar i exit
#include <iomanip> //biblioteka do manipulaowania wejściem i wyjściem
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#endif
#include <cstdlib> //biblioteka, aby działały komendy sytemowe np: system("cls");
#include <time.h>
#include <fstream> //biblioteka do obsługi plików
#include <cmath>
#include <algorithm>
#endif


using namespace std;

class text_analyzer{
  private:

  int _position = 0;
  #if arduino
  void (*_errorFunc)(String text);
  #else
  void (*_errorFunc)(string text);
  #endif

  #if arduino
  bool special_char_in_correct_place(String &data, unsigned int position);
  #else
  bool special_char_in_correct_place(string &data, int position);
  #endif
  bool correctBracketQuantity(uint8_t *bracketQuantity, char c);

  public:

    bool *error;

    #if arduino
    void addErrorFuntion(void (*errorFunction)(String text));
    #else
    void addErrorFuntion(void (*errorFunction)(string text));
    #endif
    
    #if arduino
    String rewrite_in_range(uint8_t start, uint8_t end, String data) {
      String new_data = "";
    #else
    string rewrite_in_range(uint8_t start, uint8_t end, string data) {
      string new_data = "";
    #endif
      if (end < 0) {
        return "";
      }
      for (uint8_t i = start; i <= end; i = i + 1) {
        new_data += data[i];
      }
      return new_data;
    }

    #if arduino
    void delete_spaces(String &data) {
      String new_data = "";
    #else
    void delete_spaces(string &data) {
      string new_data = "";
    #endif
      uint8_t i = 0;
      char analize_char = data[0];
      while (analize_char == ' ') {
        analize_char = data[++i];
      }

      for (uint8_t j = i; j < data.length(); j++) {
        new_data += data[j];
      }
      data = new_data;
    }

    #if arduino
    String rm_comment(String data) {
      if(data.indexOf("REM")==-1){
    #else
    string rm_comment(string data) {
      if(data.find("REM")==string::npos){
    #endif
        return data;
      }
      #if arduino
      String new_data = rewrite_in_range(0, data.indexOf("REM")-1, data);
      #else
      string new_data = rewrite_in_range(0, data.find("REM")-1, data);
      #endif
      return new_data;
    }

    #if arduino
    String trim(String data){
    #else
    string trim(string data){
    #endif
      int data_len = data.length();
      char sign=data[data_len-1];
      while(sign==' '){
        data_len--;
        sign=data[data_len-1];
      }
      return rewrite_in_range(0, data_len-1, data);
    }

    #if arduino
    void up_case(String &data){
      String new_data="";
    #else
    void up_case(string &data){
      string new_data="";
    #endif
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

    #if arduino
    void delete_useless_spaces(String &data);

    String one_word_ret(String data);

    String reform_input(String &data);
    #else
    void delete_useless_spaces(string &data);

    string one_word_ret(string data);

    string reform_input(string &data);
    #endif
};

#endif
