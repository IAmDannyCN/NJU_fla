; This program does multiplication
; Input: a string of a's and then b's, e.g. 'aaabb'

; the finite set of states
#Q = {0,read_a,read_b,illegal_input,illegal1,illegal2,illegal3,illegal4,illegal5,illegal6,illegal7,illegal8,illegal9,illegal10,illegal11,illegal12,illegal_halt,calc,calc_backb,halt_accept}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,_,c,i,l,e,g,n,p,u,t}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

; check illegal input
0 ___ ___ *** illegal_input ; empty input
0 b__ ___ r** illegal_input ; `a` not found
0 a__ _a_ rr* read_a
read_a a__ _a_ rr* read_a
read_a ___ ___ *** illegal_input ; `b` not found
read_a b__ __b r*r read_b
read_b b__ __b r*r read_b
read_b ___ ___ *ll calc
read_b a__ ___ r** illegal_input ; unexpected `a` after `b`

; handle illegal_input
illegal_input a__ ___ r** illegal_input
illegal_input b__ ___ r** illegal_input
illegal_input ___ i__ r** illegal1
illegal1 ___ l__ r** illegal2
illegal2 ___ l__ r** illegal3
illegal3 ___ e__ r** illegal4
illegal4 ___ g__ r** illegal5
illegal5 ___ a__ r** illegal6
illegal6 ___ l__ r** illegal7
illegal7 ___ ___ r** illegal8
illegal8 ___ i__ r** illegal9
illegal9 ___ n__ r** illegal10
illegal10 ___ p__ r** illegal11
illegal11 ___ u__ r** illegal12
illegal12 ___ t__ *** illegal_halt

; calc
calc __b *** l** halt_accept
calc _ab c** r*l calc
calc _a_ *** **r calc_backb
calc_backb _ab *** **r calc_backb
calc_backb _a_ *** *ll calc