; FROM: https://github.com/UshioA/turing-project/blob/main/testcases/binarymult.tm
; THIS TEST REQUIRES YOU TO HANDLE MULTIPLE POSSIBLE DELTA LIKE THIS:
; delta1 = 3 a a r 4
; delta2 = 3 * * l 3

; Binary multiplication machine - multiplies two numbers given in binary
; Input: two numbers in binary, separated by a x, eg "101x1101"
; Output: the product of the two inputs, in binary

; The machine stores data on the tape in the configuration "tally num1 num2" where num1, num2 are the input and tally is the running total for the product.

; the finite set of states
#Q = {0,1,2,3,8,9,10,11,20,21,25,26,30,31,32,40,41,42,43,44,45,46,47,48,50,51,52,53,54,55,56,57,58,59,70,71,72,73,74,75,76,80,81,82,halt}

; the finite set of input symbols
#S = {0,1,x}

; the complete set of tape symbols
#G = {0,1,_,x,o,i}

; the start state
#q0 = 8

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 1

8 _ _ * 0
8 0 0 r 8
8 1 1 r 8
8 x _ l 9
9 0 0 l 9
9 1 1 l 9
9 _ _ r 0

; Set up tally
0 _ _ l 1
0 * * l 1
1 _ _ l 2
2 _ 0 r 3
3 _ _ r 10

; Find end of num1
10 _ _ l 11
10 x x l 11
10 0 0 r 10
10 1 1 r 10


; If last digit of num1 is 0, multiply num2 by 2
11 0 x r 20
; If last digit of num1 is 1, add num2 to tally and then multiply num2 by 2
11 1 x r 30


; Multiply num2 by 2
20 _ _ r 20
20 x x r 20
20 * * r 21
21 _ 0 l 25 ; Multiplication by 2 done, return to end of num1
21 * * r 21
25 _ _ l 26
25 * * l 25
26 _ _ r 80 ; Finished multiplying. Clean up
26 x x l 26
26 0 0 * 11
26 1 1 * 11

; Add num2 to tally
30 _ _ r 30
30 x x r 30
30 * * r 31
31 _ _ l 32
31 * * r 31
32 0 o l 40 ; Add a zero
32 1 i l 50 ; Add a one
32 o o l 32
32 i i l 32
32 _ _ r 70 ; Finished adding

; Adding a 0 to tally
40 _ _ l 41
40 * * l 40 ; Found end of num2
41 _ _ l 41
41 * * l 42 ; Found start of num1
42 _ _ l 43 ; Found end of num1
42 * * l 42
43 o o l 43
43 i i l 43
43 0 o r 44
43 1 i r 44
43 _ o r 44
44 _ _ r 45 ; Found end of tally
44 * * r 44
45 _ _ r 45
45 * * r 46 ; Found start of num1
46 _ _ r 47 ; Found end of num1
46 * * r 46
47 _ _ r 47
47 * * r 48
48 _ _ l 32 ; Found end of num2
48 * * r 48

; Adding a 1 to tally
50 _ _ l 51 ; Found end of num2
50 * * l 50 
51 _ _ l 51
51 * * l 52 ; Found start of num1
52 _ _ l 53 ; Found end of num1
52 * * l 52
53 o o l 53
53 i i l 53
53 _ i r 55
53 0 i r 55 ; return to num2
53 1 o l 54
54 0 1 r 55
54 1 0 l 54
54 _ 1 r 55
55 _ _ r 56 ; Found end of tally
55 * * r 55
56 _ _ r 56
56 * * r 57 ; Found start of num1
57 _ _ r 58 ; Found end of num1
57 * * r 57
58 _ _ r 58
58 * * r 59
59 _ _ l 32 ; Found end of num2
59 * * r 59

; Finished adding, clean up
70 i 1 r 70
70 o 0 r 70
70 _ _ l 71
71 _ _ l 72 ; Found end of num2
71 * * l 71
72 _ _ l 72
72 * * l 73 ; Found start of num1
73 _ _ l 74
73 * * l 73
74 o 0 l 74
74 i 1 l 74
74 _ _ r 75
74 * * r 75 ; Finished cleaning up tally
75 _ _ r 76
75 * * r 75
76 _ _ r 20 ; Multiply num2 by 2
76 * * r 76

; Finished multiplying, clean up
80 x _ r 80
80 _ _ r 81
81 _ _ l 82
81 * _ r 81
82 _ _ l 82
82 * * * halt