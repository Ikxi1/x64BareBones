GLOBAL loadIDT
GLOBAL saveIDT
GLOBAL _Cli
GLOBAL _Sti
GLOBAL PIC1mask
GLOBAL PIC2mask

saveIDT:
    mov EAX, [ESP+4]	;Store in EAX the 6 byte array address
    sidt [EAX]			;Store in the IDT the array
    ret

loadIDT:
    lidt    [ESP+4]		; Done loading the IDT
    ret

_Cli:
    cli				; Disable interruptions
    ret

_Sti:
    sti				; Enable interruptions
    ret

PIC1mask:				; Writes PIC1 Mask
    push    rbp
    mov     rbp, rsp
    mov     ax, [rbp+8]  ; ax = 16 bit mask
    out     0x21, al
    pop     rbp
    retn

PIC2mask:				; Writes PIC1 Mask
    push    rbp
    mov     rbp, rsp
    mov     ax, [ebp+8]	; ax = 16 bit mask
    out     0xA1, al
    pop     rbp
    retn

