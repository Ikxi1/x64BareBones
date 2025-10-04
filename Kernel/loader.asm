GLOBAL _loader
EXTERN _main
extern init_int
extern init_LUT

STACKSIZE equ 0x4000        ; that's 16k.

_loader:
    mov qword [VesaModeInfoBlockBuffer], rax

    mov Rsp, stack+STACKSIZE; set up the stac
    cli
    call init_int
    call init_LUT   ; populate the keyboard scancode LUT
    sti
    ; call _main      ; call kernel proper
.L1:
    hlt             ; halt machine should kernel return
    JMP .L1

VesaModeInfoBlockBuffer db 256

eokl    dd STACKSIZE + stack
section .bss
    align 32
stack:
    resb STACKSIZE     ; reserve 16k stack on a quadword boundary
