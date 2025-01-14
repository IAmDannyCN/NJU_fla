; This program checks the corner cases for the parser
; Input: No input
; Expected output (when no error): p

; USAGE: Choose ONE AND ONLY ONE from each group

; #Q = {start,accept}           ; no error
; #Q= {  start,      accept }   ; no error
; # Q={start,accept}            ; no error
; #Q = {start,accept,}          ; syntax error
; #Q = {start,accept,           ; syntax error
; #Q = start,accept}            ; syntax error
; #Q = start,accept             ; syntax error
; #Q = {\n,start,accept}        ; syntax error
; #Q = {[,start,accept}         ; syntax error

; #S                    ={}     ; no error
; #S = {,l}                     ; syntax error
; #S = }                        ; syntax error
; #S = {\n}                     ; syntax error
; #S = {{,}}                    ; syntax error
; #S = {,}                      ; syntax error
; #S = {,,,}                    ; syntax error
; #S = {[,]}                    ; no error

; #G = {_,p}                    ; no error
; #G = {p}                      ; syntax error

; #q0=start                     ; no error
; #q0 = q0                      ; syntax error

; #B =_                         ; no error
; #B = p                        ; syntax error
; #B = ,                        ; syntax error

; #F = {accept}                 ; no error
; #F = {}                       ; no error
; #F = {yidalimian}             ; syntax error

; #N=1                          ; no error
; #N = 0                        ; syntax error
; #N = 3000                     ; syntax error
; #N = sishierhaohunningtu      ; syntax error

; start _ p * accept            ; no error
; start _ p < accept            ; syntax error