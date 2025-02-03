[BITS 64]
DEFAULT REL

EXTERN PreMain

GLOBAL Start

[SECTION .text$A]

    Start:
        push  rsi
        mov   rsi, rsp
        and   rsp, 0FFFFFFFFFFFFFFF0h
        sub   rsp, 020h
        call  PreMain
        mov   rsp, rsi
        pop   rsi
        ret


[SECTION .text$E]

    SymEnd:
        db 'E', 'N', 'D', 'O', 'F', 'C', 'O', 'D', 'E'