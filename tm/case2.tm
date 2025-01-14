; This program decides a perfect square
; Input: a string of 1's, e.g. '111111111'

; the finite set of states
#Q = {0,check,add,rev,true,true1,true2,true3,false,false1,false2,false3,false4,halt_reject,halt_accept}

; the finite set of input symbols
#S = {1}

; the complete set of tape symbols
#G = {_,1,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

0 __ ** ** false
0 1_ 11 ** check

check _1 ** ** false
check 1_ *1 *r add
add 1_ *1 ** rev
rev 11 ** *l rev
rev 1_ ** *r check
check 11 _* rr check
check __ __ ** true

false ** _* r* false
false _* f* r* false1
false1 _* a* r* false2
false2 _* l* r* false3
false3 _* s* r* false4
false4 _* e* ** halt_reject

true __ t_ r* true1
true1 __ r_ r* true2
true2 __ u_ r* true3
true3 __ e_ ** halt_accept