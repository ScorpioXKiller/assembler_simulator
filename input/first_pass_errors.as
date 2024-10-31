;comment
.define  sz = 2
LOOP:   mov r1,
MAIN:   mov  r3, LISTsz
        jmp  #2
        prn  #-5
        mov  STR[5], STR[2]
        sub  r1 r4
L1:     inc  K, U
        bn  LOOP
END:    hlt
.define len = 4
STR:    .string "abcdef
LIST:   .data 6, -9, END
K:      .data 2.2