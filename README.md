# BICPP-Basic_Interpreter_in_CPP
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
Integer variable in the basic interpreter is marked by *%* on the end of name. For example:
##### `INTVAR%`
If you want to write integer in hexadecimal code you must write at the end of constant letter *H*. For example: *A1H* is *161* in decimal code.  
Float variables and constants are known in Basic language as *numeric*, which aren't marked by any char at the end. For exapmple:
##### `NUMVAR`
Integer and numeric variable or constants have the same operator, which are:
- *+* add two numbers (e.g.: `2+2` returns `4`);
- *-* substract two numbers (e.g.: `4-2` returns `2`);
- */* divide two numbers (e.g.: `8/2` returns `4`);
- *\** multiply two numbers (e.g.: `8*2` returns `16`);
- *^* raise a number to power (e.g.: `8^2` returns `64`);

 **Attention!** If expression consist of integer result will be integer so you lose information after the decimal separator e.g.: `7/2` result is `3` not `3.5`. If you want, that result will be numeric you must write it as numeric e.g.: `7.0/2.0` returns `3.5`.  
 You can place expression in round brackets, which influencs sequences of operations e.g.: `2+2/2` returns `3` but  `(2+2)/2` returns `2`.

String is text, which must be enclosed in a pair of *"* and it's doesn't changed by interpreter to capital letter and more than one space aren't deleted. If you want create string variable, name of it must marked by *$* on the end. For example:
##### `STRVAR$`
If you want add *"* in the text you must write *"* before *"*. For example:
##### `" If you want print ""Hello World"" on terminal you must use the command: ""PRINT ""Hello World"""`
String operator are:
-   *&* which concatenate text;
    If you write it in command line:
    ##### `"abcd"&"abcd"`
    you get:
    ##### `abcdabcd`
-   *[n:m]* which returns substring from text for *n* to *m* char, where first char has number *0* e.g.: `"abcd"[1:2]` returns `bc`

Array can be int, string and numeric and at end of name, but before bracker, you must give the same marked. For example:
##### `STRARRAY$(4)`
It's array, which store five strings. If you want create 2 dimensional array you can do it at the way:
##### `STRARRAY$(4,2)`
The array consist of 5 rows and 3 columns.

Integer, numeric and string value can be compared by relational operator, which are:
- *>* returns true, when left value is bigger than right;
- *>=* returns true, when left value is bigger or equal to right;
- *<* returns true, when right value is bigger than left;
- *<=* or *=<* returns true, when right value is bigger or equal left;
- *=* or *==* returns true, when left value is equal to right;
- *<>* returns true, when left value is unequal to right;

You must remember, that numeric value are converted to int value before compare.

Result of relation operations or numbers can be checked by logical operators, which are:
- *NOT* deny term (e.g.: `NOT 1` return `0`);
- *AND* return `1` only if left and right term is `1` (e.g.: `1 AND 0` return `0` but `1 AND 1` return `1`);
- *OR* return `1` if even one term is `1` (e.g.: `1 OR 1` return `1` and `1 OR 0` return `1`);
- *XOR* return `1` if one term is `1` (e.g.: `1 XOR 1` return `0` but `1 XOR 0` return `1`);
- *IMP* return `1` if two terms are `1` or `1` or first terms is `1` and second term is `1` (e.g.: `0 IMP 0` return `1` and `0 IMP 1` return `1` but `1 IMP 0` return `0`);
- *EQV* return `1` only if two term are `1` or `1` (e.g.: `1 EQV 1` return `1` and `1 EQV 0` return `0`).

#### Statements

At the moment there are the following statements:
- `LET`
- `REM`
  