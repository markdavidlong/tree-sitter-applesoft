1 REM TEST PROGRAM
2 REM ERRORS MAY BE DELIBERATELY INTRODUCED TO TEST THE PARSER
10 A$ = "HELLO" + " " + "WORLD."
20 PRINT LEFT$(A$,11)
30 A = 1: B = -2
40 C = ABS(A+B)
50 PRINT "ABS(";A;" + ";B;") = ";C
60 END
