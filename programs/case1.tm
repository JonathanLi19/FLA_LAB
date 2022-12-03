; 实现二进制数的循环右移操作
; Input: a string of 0's and 1's, e.g. '1001001'

; the finite set of states
#Q = {0,cp,cmp,mh,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {0,1,_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions