# BreadcrumbsBICPP-Basic_Interpreter_in_CPP
## Description
It's basic interpreter written in C++.
## Introduction

To compile project advised is *gcc* compiler. It doesn't required any libraries to compile. You can compile project by downloading it, opening folder. Next you should open terminal and print the command (linux or windows)
##### `g++ main.cpp -o main.exe`
To run compiled program in windows you can do it by double click *main.exe* in linux you must print the command (if you are in the folder with project).
##### `.\main.exe`
After runing compiler you should see information bellow.
```
Basic interpreter example program
>>>
```
You can print commands!
If you want interrupt program working you can do it by typing *ESC* key.

### Syntax

In basic language commands should by written with capital letters, but in the version you don't have to do it, because interpreter do it.
Interpreter works in immedidate and execution mode. In immediate mode program is executed without delay. If you want work in the mode you must write command without number line for example
##### `PRINT "HI"`
In execution before command must be line number.
##### `10 PRINT "HI"`

#### Variables
Integer in the basic interpreter be in range from -32767 to 32767.
If you want to write integer in hexadecimal code you must write at the end *H*. For example: *A1H* is *161* in decimal code.
String is text, which must be enclosed in a pair of *"* and it's doesn't changed by interpreter to capital letter and more than one space aren't deleted.