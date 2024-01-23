clear
10 print "arythmetic expressions"
20 print 1+1
30 print (1.0+1.0)
40 print 2+2/2
50 print (2.0+2.0/2.0)
60 print (2+2)/2
70 print (2.0+2.0)/2.0
80 print n
run

clear
10 print "let statement"
20 let x = 2
30 print x
40 let x = 1
50 print x
60 let x = x+2
70 print x
80 let y = 2
90 print y
100 let x = x+y
110 print x
120 let x% = 2
130 print x%
140 let x% = x%+x
150 print x%
run

clear
10 print  "string"
20 print "napis"
30 let x$ = "abcdefghijklmnopqrstuvwxyz"
40 print x$&"1234"
50 print x$[0:2]
60 print x$[2:0]
run

clear
10 print "relation and logical operators"
20 print 10>2, "\t", 10>100
30 print 10>=10, "\t", 10>100
40 print 10=<10, "\t", 100<10
50 print 10=10, "\t", 10<>100
60 print "not: ", not 1, "\t", not 0
65 print "   \t","1x1\t0x0\t1x0\t0x1"
70 print "or:\t", 1 or 1, "\t", 0 or 0, "\t", 1 or 0
80 print "xor:\t", 1 xor 1, "\t", 0 xor 0, "\t", 1 xor 0
90 print "and:\t", 1 and 1, "\t", 0 and 0, "\t", 1 and 0
100 print "imp:\t", 1 imp 1, "\t", 0 imp 0, "\t", 1 imp 0, "\t", 0 imp 1
110 print "eqv:\t", 1 eqv 1, "\t", 0 eqv 0, "\t", 1 eqv 0
run

clear 
10 input "give radius: " r
20 let pi = 3.14
30 let area = pi*r^2
40 print "area of the circle is: ", area
50 goto 10
list
run

