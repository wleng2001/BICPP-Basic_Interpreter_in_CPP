#include "basic.h"
#include <Arduino.h>

String readSerial(){
    String text;
    while(Serial.available()>0){
        text+=Serial.readStringUntil('\n');
    }
    return text;
}

void printSerial(String *text){
    Serial.print(*text);
}

void errorSerial(String text){
    Serial.print(text);
}

basic bas(printSerial, errorSerial, readSerial);

void setup(){
    Serial.begin(9600);
    Serial.println("Basic interpreter example implementation");
    Serial.println("Version: "+String(_basicVersion));
}

void loop(){
    String text = readSerial();
    if(text!= ""){
        Serial.println(bas.run(&text));
    }

}