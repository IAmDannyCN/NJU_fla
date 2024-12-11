; This program writes X for 10000 times
; Test: 1. Empty set definition. Such as: #S = {}
;       2. Long tape. Testing for potential illegal iterator when allocating space for tape_t through std::__allocate
;       3. Empty input.
; Expected Output: X (for 10000 times)

; the finite set of states
#Q = {0,count,carry1,carry2,carry3,carry4,halt}

; the finite set of input symbols
#S = {}

; the complete set of tape symbols
#G = {_,0,1,2,3,4,5,6,7,8,9,X,c}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt}

; the number of tapes
#N = 5

; the transition functions
0 _____ _0000 ***** count
count _**** c**** ***** carry1

carry1 c***0 X***1 r**** count
carry1 c***1 X***2 r**** count
carry1 c***2 X***3 r**** count
carry1 c***3 X***4 r**** count
carry1 c***4 X***5 r**** count
carry1 c***5 X***6 r**** count
carry1 c***6 X***7 r**** count
carry1 c***7 X***8 r**** count
carry1 c***8 X***9 r**** count
carry1 c***9 c***0 ***** carry2

carry2 c**0* X**1* r**** count
carry2 c**1* X**2* r**** count
carry2 c**2* X**3* r**** count
carry2 c**3* X**4* r**** count
carry2 c**4* X**5* r**** count
carry2 c**5* X**6* r**** count
carry2 c**6* X**7* r**** count
carry2 c**7* X**8* r**** count
carry2 c**8* X**9* r**** count
carry2 c**9* c**0* ***** carry3

carry3 c*0** X*1** r**** count
carry3 c*1** X*2** r**** count
carry3 c*2** X*3** r**** count
carry3 c*3** X*4** r**** count
carry3 c*4** X*5** r**** count
carry3 c*5** X*6** r**** count
carry3 c*6** X*7** r**** count
carry3 c*7** X*8** r**** count
carry3 c*8** X*9** r**** count
carry3 c*9** c*0** ***** carry4

carry4 c0*** X1*** r**** count
carry4 c1*** X2*** r**** count
carry4 c2*** X3*** r**** count
carry4 c3*** X4*** r**** count
carry4 c4*** X5*** r**** count
carry4 c5*** X6*** r**** count
carry4 c6*** X7*** r**** count
carry4 c7*** X8*** r**** count
carry4 c8*** X9*** r**** count
carry4 c9*** X**** ***** halt