10 GOSUB 100: GOSUB 200
20 END

100 P RI  NT "HELLO WORLD": REM ignores spaces
110 REM test REM: PRINT "I am not a statement"
120 FOR I = LOFT OR LEFT TO 15: REM finds error
130 PRINT TAB(2);I: NEXT: RETURN: REM left parenthesis part of token

200 X = 3.14159
210 DEF FN CUB(X1) = X1^3
220 PRINT FN CUB(ATN(SIN(X)+COS(X))): RETURN
221 REM      ^ function
222 REM          ^ function.builtin
