#include "text_analyzer.h"

char text_analyzer::mathematic_char[] = {
  '+', '-', '%', '*', '/', '<', '>', '='
};

void text_analyzer::delete_useless_spaces(string &data) { //uses all option of the class
  data=text_analyzer::trim(data);
  text_analyzer::delete_spaces(data);
  data = text_analyzer::rm_comment(data);
  bool pre_spaces = false;
  bool start_quote = false;
  bool start_comment=false;
  uint8_t i = 0;
  string temporary_data = "";
  while (i < data.length()) {
    if (data[i] == '"') {
      start_quote = !start_quote;
    }
    if(data[i] == 0x09){
      data[i] = ' ';
    }
    if(data[i] == ';' && (data[i-1] == '\n' || i==0)){
      start_comment= !start_comment;
    }
    if(toupper(data[i-1])=='M' && toupper(data[i-2])=='E' && toupper(data[i-3])=='R'){
      start_comment = true;
    }
    if(start_comment==true && data[i]=='\n'){
      start_comment = false;
    }
    if (data[i] == ' ' && start_quote == false && start_comment == false && pre_spaces == true) {
      //data=text_analyzer::rewrite_in_range(0,i,data)+text_analyzer::delete_spaces(text_analyzer::rewrite_in_range(i,data.length(),data));
    } else {
      if(start_comment==true || start_quote==true){
        temporary_data+=data[i];
      }else{
        temporary_data += toupper(data[i]);
      }
      if (data[i] == ' ') {
        pre_spaces = true;
      } else {
        pre_spaces = false;
      }
    }
    i++;
  }
  data = temporary_data;
}

string text_analyzer::one_word_ret(string data){
  string comm="";
  bool start_quote=false;
  if(data[0]=='"'){
    start_quote!=start_quote;  
  }
  for(uint8_t i=0; i<data.length();i++){
    if(i>=data.length() or (data[i]==' ' and start_quote==false)){
      transform(comm.begin(), comm.end(), comm.begin(), ::toupper);
      return comm;
    }
    if(data[i]=='"'){
      start_quote!=start_quote;  
    }
    comm+=data[i];
  }
  transform(comm.begin(), comm.end(), comm.begin(), ::toupper);
  return comm;
}

string text_analyzer::reform_input(string &data){
  text_analyzer::delete_useless_spaces(data);
  int data_len = data.length();
  size_t position = 0;

  //edit passage with '\' (it's char of new statement)

  position = data.find( '\\', position);
  while(position != std::string::npos){
    if(data[position-1]!=' ' && (position-1)>=0){
      data.insert(position, " ");
      position++;
    }
    if(data[position+1]!=' '){
      data.insert(position+1, " ");
      position++;
    }
    data_len = data.length();
    position = data.find('\\', position+1);
  }

  //edit passage with mathematic char

  for(int i = 0; i<sizeof(text_analyzer::mathematic_char)/sizeof(text_analyzer::mathematic_char[0]); i++){
    position = 0;
    position = data.find(mathematic_char[i], position);
    while(position!=std::string::npos && position<data_len){
      if(data[position-1]==' ' && (position-1)>=0){
        data.erase(position-1, 1);
        position--;
      }
      if(data[position+1]==' '){
        data.erase(position+1, 1);
      }
      data_len = data.length();
      position = data.find(mathematic_char[i], position+=2);
    }
  }

  //edit passage with variable assigment

  position = 0;
  position = data.find( '=', position);
  while(position != std::string::npos){
    if(data[position-1]!=' ' && (position-1)>=0 && data[position-1]!='>' && data[position-1]!='<'){
      data.insert(position, " ");
      position++;
    }else{
      if(data[position-1]=='>' || data[position-1]=='<'){
        position = data.find('=', position+1);
        continue;
      }
    }
    if(data[position+1]!=' '){
      data.insert(position+1, " ");
      position++;
    }
    data_len = data.length();
    position = data.find('=', position+1);
  }

  return data;
}