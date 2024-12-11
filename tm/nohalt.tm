; This program goes right forever
; Expected Behavior: It should not halt in a few seconds.

; the finite set of states
#Q = {0,1}

; the finite set of input symbols
#S = {z}

; the complete set of tape symbols
#G = {_}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {0}

; the number of tapes
#N = 1

; the transition functions
0 _ * r 1
1 _ * l 0