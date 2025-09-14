GLOBAL _loader
EXTERN _main

STACKSIZE equ 0x4000		; that's 16k.

_loader:
    mov Rsp, stack+STACKSIZE; set up the stac

    call _main		; call kernel proper
.L1:
    hlt			; halt machine should kernel return
    JMP .L1

eokl	dd STACKSIZE + stack
    section .bss
    align 32
stack:
    resb STACKSIZE		; reserve 16k stack on a quadword boundary
