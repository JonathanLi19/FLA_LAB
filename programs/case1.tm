; 实现二进制数的循环右移操作
; Input: a string of 0's and 1's, e.g. '1001001'

; the finite set of states
#Q = {0,cp,mh1,cp1,mh2,cp2,clean,halt}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 0_ 0_ ** cp
0 1_ 1_ ** cp
0 __ __ ** halt ; empty input

; State cp: copy the string to the 2nd tape 
cp 0_ 00 rr cp
cp 1_ 11 rr cp
cp __ __ ll mh1

; State mh1: move 1st head to the left
mh1 00 00 l* mh1
mh1 01 01 l* mh1
mh1 10 10 l* mh1
mh1 11 11 l* mh1
mh1 _0 _0 r* cp1
mh1 _1 _1 r* cp1

; State cp1: copy 2nd head's symbol to 1st head's symbol
cp1 *0 00 rl mh2
cp1 *1 11 rl mh2

; State mh2: move 2nd head to the left
mh2 00 00 *l mh2
mh2 01 01 *l mh2
mh2 10 10 *l mh2
mh2 11 11 *l mh2
mh2 0_ 0_ *r cp2
mh2 1_ 1_ *r cp2

; State cp2: copy 2nd tape to 1st tape
cp2 *0 00 rr cp2
cp2 *1 11 rr cp2
cp2 _0 _0 l* clean
cp2 _1 _1 l* clean

; State clean: clean 2nd tape
clean ** *_ *l clean
clean *_ *_ *r halt