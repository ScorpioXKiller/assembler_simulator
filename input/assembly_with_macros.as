.define  sz = 2
MAIN:   mov  r3, LIST[sz]
                         
LOOP:   jmp  L1
        mcr m1_mcr
            cmp  r3, #sz
            bne  END
        endmcr
        mcr m2_mcr
            cmp r2, #sz
            mov r2, LIST[sz]
            bne END
        endmcr
        prn  #-5 
        mov  STR[5], STR[2]
        sub  r1, r4
        m1_mcr
L1:     inc  K
                 
        bne  LOOP
END:    hlt
.define len = 4
        
STR:    .string "abcdef"

LIST:   .data 6, -9, len
        m1_mcr
K:      .data 22