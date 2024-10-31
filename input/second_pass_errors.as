.entry LIST
.extern W
LOOP:   mov r1, r4
.define  sz = 2
MAIN:   mov  r3, LIST[sz]
LOOP:   jmp  W
        prn  #-5
        mov  ARR[5], ARR[2]
K:      jmp r1
LOOP:   sub r3, r4
        sub  r1, r4
        cmp K, #sz
        bne W
L1:     inc  L4
.entry LOOP
        bne  LOOP
END:    hlt
.define len = 4
STR:    .string "abcdef"
K:      .data 22
ARR:    inc K
.extern L3