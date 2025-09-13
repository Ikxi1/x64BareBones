GLOBAL syscall
GLOBAL scyield
GLOBAL scSwitchToCurrent

syscall:
    push rBP
    mov EBP, ESP

    mov EAX, [EBP + 8]
    mov EBX, [EBP + 12]
    mov ECX, [EBP + 16]
    mov EDX, [EBP + 20]
    mov ESI, [EBP + 24]
    mov EDI, [EBP + 28]

    int 80h

    pop rBP
    ret

;Force a context switch
scyield:
    int 81h
    ret
