GLOBAL _loader
GLOBAL eokl			; end of kernel land
EXTERN _main


; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN	equ		1<<0					; align loaded modules on page boundaries
MEMINFO		equ		1<<1					; provide memory map
FLAGS		equ		MODULEALIGN | MEMINFO	; this is the Multiboot 'flag' field
MAGIC		equ		0x1BADB002				; 'magic number' lets bootloader find the header
CHECKSUM	equ -(MAGIC + FLAGS)			; checksum required

section .text
align 4
MultiBootHeader:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

    ; reserve initial kernel stack space
    STACKSIZE equ 0x4000		; that's 16k.

_loader:
    mov esp, stack+STACKSIZE; set up the stack
    push rax		; pass Multiboot magic number
    push rbx		; pass Multiboot info structure

    call  _main		; call kernel proper
    hlt			; halt machine should kernel return

eokl	dd STACKSIZE + stack
    section .bss
    align 32
    stack:
    resb STACKSIZE		; reserve 16k stack on a quadword boundary
