; 判断输入串的长度是否为平方数
; Input: a string of 1's

; the finite set of states
#Q = {0,s1,s2,s3,s4,accept,reject,halt_accept,halt_reject,accept2,accept3,accept4,reject2,reject3,reject4,reject5}

; the finite set of input symbols
#S = {1}

; the complete set of tape symbols
#G = {X,1,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept,halt_reject}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state -- if empty halt_true, else add a marker X to 2nd tape
0 *_ *X ** s1
0 __ __ ** accept ; empty input

; State s1: move head0 and head1 a position right, write blank to 1st tape as the head scans it
s1 ** _* rr s2

; State s2: check the symbol under both heads
s2 __ __ ** accept
s2 *_ *X *r s3
s2 _* _* ** reject
s2 ** ** ** s1 

; State s3: add a X to 2nd tape
s3 *_ *X *l s4

; State s4: move 2nd head to the left
s4 ** ** *l s4
s4 *_ *_ *r s1

; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State reject*: write 'false' on 1st tape
reject _* f_ r* reject2
reject __ f_ r* reject2
reject2 _* a_ r* reject3
reject2 __ a_ r* reject3
reject3 _* l_ r* reject4
reject3 __ l_ r* reject4
reject4 _* s_ r* reject5
reject4 __ s_ r* reject5
reject5 _* e_ ** halt_reject
reject5 __ e_ ** halt_reject