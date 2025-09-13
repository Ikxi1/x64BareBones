GLOBAL loadGDT
GLOBAL saveGDT
GLOBAL loadGDTest

loadGDTest:
    mov AX, [RSP+16]      ; specified.
    ret

saveGDT:
    mov RAX, [RSP+8]        ;Store in EAX the 6 byte array address
    sgdt [RAX]              ;Store in the GDT the array
    ret

loadGDT:
    lgdt    [Rsp+8]         ; Done loading the GDT
    jmp		[jmpfar]        ; Cable que no tuve forma de sacar T__T
loadCS:
    mov     AX, [rSP+16]  ; Data Selector
    mov     DS, AX
    mov     ES, AX
    mov     FS, AX
    mov     GS, AX
    mov     SS, AX
    ret

jmpfar:
    dq loadCS
    dw 0x08