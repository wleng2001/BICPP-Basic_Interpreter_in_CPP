#include "text_analyzer.h"

void text_analyzer::addErrorFuntion(void (*errorFunction)(string text)){
  text_analyzer::_errorFunc = errorFunction;
}

bool text_analyzer::special_char_in_correct_place(string &data, int position){
  for(auto i = 0; i < sizeof(specialChar) / sizeof(specialChar[0]); i++){
    if(specialChar[i]==data[position]){
      char c = data[position+1];
      if((c==' ' || isMathematicChar(data[position+1]) || c=='[' || position==data.length()-1 || c=='&' || c==',') && (isalnum(data[position-1]))){
        break;
      }else{

        _errorFunc("Error: invalid syntax (char: "+to_string(position)+"): "+data);
        *error = true;
        return false;
      }
    }
  }
  return true;
}

bool text_analyzer::correctBracketQuantity(uint8_t *bracketQuantity, char c){
  if(c=='('){
    *bracketQuantity+=1;
    return true;
  }
  if(c==')'){
    if(*bracketQuantity==0){
      return false;
    }else{
      *bracketQuantity-=1;
      return true;
    }
  }
  return true;
}

void text_analyzer::delete_useless_spaces(string &data) { //uses all option of the class
  
  data=text_analyzer::trim(data);
  text_analyzer::delete_spaces(data);
  data = text_analyzer::rm_comment(data);
  bool pre_spaces = false;
  bool start_quote = false;
  bool start_comment=false;
  uint8_t bracketQuantity = 0;
  uint8_t i = 0;
  string temporary_data = "";
  bool quotation_in_text = false;
  while (i < data.length()) {
    char c = data[i];
    #if debug
    cout << "i: " << to_string(i) << " c: " << c << endl;
    #endif
    if(!start_comment && !start_quote){
      if(!correctBracketQuantity(&bracketQuantity, c)){
        _errorFunc("ERROR: too little open bracket (char: "+to_string(i)+"): "+data);
        *error = true;
        return;
      }
      if(!special_char_in_correct_place(data, i))
        return;
    }
    if(c == '"' && (!data[i-1] == '"' != !data[i+1] == '"')){
      quotation_in_text = true;
    }else{
      quotation_in_text = false;
    }
    if (c == '"' && quotation_in_text == false) {
      start_quote = !start_quote;
    }
    if(c == '\t'){
      data[i] = ' ';
    }
    if(c == ';' && (data[i-1] == '\n' || i==0)){
      start_comment= true;
    }
    if(toupper(data[i-1])=='M' && toupper(data[i-2])=='E' && toupper(data[i-3])=='R'){
      start_comment = true;
    }
    if(start_comment==true && data[i]=='\n'){
      start_comment = false;
    }
    if (c == ' ' && start_quote == false && start_comment == false && pre_spaces == true) {
    } else {
      if(start_comment==true || start_quote==true){
        temporary_data+=c;
      }else{
        temporary_data += toupper(c);
      }
      if (c == ' ') {
        pre_spaces = true;
      } else {
        pre_spaces = false;
      }
    }
    i++;
  }
  if(bracketQuantity>0){
    _errorFunc("ERROR: too little closed bracket: "+data);
    *error = true;
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

//reform input-----------------------------------------------------

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
    if(data[position+1]!=' ' && data[position+1]!='n' && data[position+1]!='t'){
      data.insert(position+1, " ");
      position++;
    }
    data_len = data.length();
    position = data.find('\\', position+1);
  }

  //edit passage with mathematic char

  for(int i = 0; i<sizeof(mathematicChar)/sizeof(mathematicChar[0]); i++){
    position = 0;
    position = data.find(mathematicChar[i], position);
    while(position!=std::string::npos && position<data_len){
      if(data[position-1]==' ' && (position-1)>=0){
        data.erase(position-1, 1);
        position--;
      }
      if(data[position+1]==' '){
        data.erase(position+1, 1);
      }
      data_len = data.length();
      position = data.find(mathematicChar[i], position+=2);
    }
  }

  return data;
}