; FROM: https://github.com/UshioA/turing-project/blob/main/testcases/primetest.tm
; THIS TEST REQUIRES YOU TO HANDLE MULTIPLE POSSIBLE DELTA LIKE THIS:
; delta1 = 3 a a r 4
; delta2 = 3 * * l 3

; Tests if a given number is prime.
; Input: a single natural number in binary.

; This is very inefficient and slow, so be prepared to wait!

; the finite set of states
#Q = {0,1,2,3,4,9,5x,5y,6x,6y,10,11,12,13x,13y,20,21,22,23,50,51,52x,52y,53,60,61,62,63,70,71,72,80,81a,81b,81c,81dx,81dy,81ex,81ey,81f,81k,81m,81n,81o,82,83,84,85,86,87x,87y,88x,88y,90,91,100,101,102,103,200,201,202,500,501,502,502a,503,504,505,506,507,508,509,510,511,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,error,halt}

; the finite set of input symbols
#S = {0,1}

; the complete set of tape symbols
#G = {!,.,0,1,_,a,b,c,d,e,i,m,n,o,p,r,s,t,x,y}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt,error}

; the number of tapes
#N = 1

0 * * l 1
0 _ _ l 1
1 * a r 2
1 _ a r 2
2 _ b l 3
2 * * r 2
3 a a r 4
3 x x r 4
3 y y r 4
3 * * l 3
3 _ _ l 3
4 0 x r 5x
4 1 y r 5y
4 b b l 9
9 x 0 l 9
9 y 1 l 9
9 a a r 10
5x b b r 6x
5x * * r 5x
5x _ _ r 5x
5y b b r 6y
5y * * r 5y
5y _ _ r 5y
6x _ 0 l 3
6x * * r 6x
6y _ 1 l 3
6y * * r 6y
10 _ c l 11
10 * * r 10
11 b b r 12
11 x x r 12
11 y y r 12
11 * * l 11
11 _ _ l 11
12 0 x r 13x
12 1 y r 13y
12 c c l 20
13x _ 0 l 11
13x * * r 13x
13y _ 1 l 11
13y * * r 13y
20 x 0 l 20
20 y 1 l 20
20 b b r 21
21 _ d l 22
21 * * r 21
22 1 0 r 23
22 0 1 l 22
22 c ! * error
23 d d r 50
23 * * r 23
23 _ _ r 23
50 c c r 51
50 x x r 51
50 y y r 51
50 * * l 50
50 _ _ l 50
51 0 x r 52x
51 1 y r 52y
51 d d l 53
52x _ 0 l 50
52x * * r 52x
52y _ 1 l 50
52y * * r 52y
53 x 0 l 53
53 y 1 l 53
53 c c r 60
60 _ _ l 61
60 * * r 60
61 1 0 l 62
61 0 1 l 61
61 d d r 70
62 c c l 63
62 * * l 62
62 _ _ l 62
63 1 0 r 60
63 0 1 l 63
63 b b r 80
70 _ _ l 71
70 * * r 70
71 d d l 72
71 * _ l 71
71 _ _ l 71
72 c c * 50
72 * * l 72
72 _ _ l 72
80 _ _ l 81a
80 * * r 80
81a 0 1 r 81b
81a 1 0 l 81a
81a d d r 81k
81b _ _ l 81c
81b * * r 81b
81c x x l 81c
81c y y l 81c
81c d d r 100
81c 0 x l 81dx
81c 1 y l 81dy
81dx d d l 81ex
81dx * * l 81dx
81dx _ _ l 81dx
81dy d d l 81ey
81dy * * l 81dy
81dy _ _ l 81dy
81ex 0 x r 81f
81ex 1 1 r 81k
81ex x x l 81ex
81ex y y l 81ex
81ey 1 y r 81f
81ey 0 0 r 81k
81ey x x l 81ey
81ey y y l 81ey
81f _ _ l 81c
81f * * r 81f
81k _ _ l 81m
81k * * r 81k
81m d d l 81n
81m * _ l 81m
81m _ _ l 81m
81n x 0 l 81n
81n y 1 l 81n
81n 0 0 r 81o
81n 1 1 r 81o
81o d d l 82
81o * * r 81o
81o _ _ r 81o
82 1 0 l 83
82 0 1 l 82
82 c c * error
83 c c l 84
83 * * l 83
83 _ _ l 83
84 b b l 85
84 * _ l 84
84 _ _ l 84
85 a a r 86
85 x x r 86
85 y y r 86
85 * * l 85
85 _ _ l 85
86 0 x r 87x
86 1 y r 87y
86 b b l 90
87x b b r 88x
87x * * r 87x
87x _ _ r 87x
87y b b r 88y
87y * * r 87y
87y _ _ r 87y
88x _ 0 l 85
88x * * r 88x
88y _ 1 l 85
88y * * r 88y
90 x 0 l 90
90 y 1 l 90
90 a a r 91
91 c c r 51
91 * * r 91
91 _ _ r 91
100 _ _ l 101
100 * * r 100
101 d d l 102
101 * _ l 101
101 _ _ l 101
102 x 0 l 102
102 y 1 l 102
102 c c r 103
103 d d l 200
103 * * r 103
103 _ _ r 103
200 1 0 r 201
200 0 1 l 200
201 d d l 202
201 * * r 201
201 _ _ r 201
202 1 0 r 600
202 0 1 l 202
202 c c r 500
500 _ _ l 501
500 * * r 500
501 b _ l 502
501 * _ l 501
501 _ _ l 501
502 a _ r 502a
502 * * l 502
502 _ _ l 502
502a _ _ r 503
502a * * r 502a
503 * i r 504
503 _ i r 504
504 * s r 505
504 _ s r 505
505 * _ r 506
505 _ _ r 506
506 * p r 507
506 _ p r 507
507 * r r 508
507 _ r r 508
508 * i r 509
508 _ i r 509
509 * m r 510
509 _ m r 510
510 * e r 511
510 _ e r 511
511 * ! * halt
511 _ ! * halt
600 _ _ l 601
600 * * r 600
601 b _ l 602
601 * _ l 601
601 _ _ l 601
602 a _ r 603
602 * * l 602
602 _ _ l 602
603 _ _ r 604
603 * * r 603
604 * i r 605
604 _ i r 605
605 * s r 606
605 _ s r 606
606 * _ r 607
606 _ _ r 607
607 * n r 608
607 _ n r 608
608 * o r 609
608 _ o r 609
609 * t r 610
609 _ t r 610
610 * _ r 611
610 _ _ r 611
611 * p r 612
611 _ p r 612
612 * r r 613
612 _ r r 613
613 * i r 614
613 _ i r 614
614 * m r 615
614 _ m r 615
615 * e r 616
615 _ e r 616
616 * . * halt
616 _ . * halt