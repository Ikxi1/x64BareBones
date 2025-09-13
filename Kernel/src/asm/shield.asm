GLOBAL loadDirTable
GLOBAL enablePaging
GLOBAL getEIP

global read_cr0
read_cr0:
    mov rax, cr0
    retn

global write_cr0
write_cr0:
    push rbp
    mov rbp, rsp
    mov rax, [rbp+8]
    mov cr0,  rax
    pop rbp
    retn

global read_cr3
read_cr3:
    mov rax, cr3
    retn

global write_cr3
write_cr3:
    push rbp
    mov rbp, rsp
    mov rax, [rbp+8]
    mov cr3, rax
    pop rbp
    retn
