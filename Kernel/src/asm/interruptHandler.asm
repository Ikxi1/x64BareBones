GLOBAL int_09_hand
EXTERN keyboardHandler


; Handler of keyboard interrupt
int_09_hand:
    cli
    push rax

    ;in al, 0x60
    ;push rax
    ;call keyboardHandler
    ;pop rax

    mov al, 0x20 ; Envio de EOI generico al PIC
    out 0x20, al

    pop rax
;	sti			; We don't need to sti! the interruption flag is saved on stack!!
                ; Besides this way we can use nested system calls, as the
                ; corresponding flags are properly stacked

    iretq


; Build a temporary IDT
build_idt:
    cli
    cld
    xor rdi, rdi 			; create the 64-bit IDT (at linear address 0x0000000000000000)

    mov rcx, 32
make_exception_gates: 			; make gates for exception handlers
    mov rax, exception_gate
    push rax			; save the exception gate to the stack for later use
    stosw				; store the low word (15..0) of the address
    mov ax, SYS64_CODE_SEL
    stosw				; store the segment selector
    mov ax, 0x8E00
    stosw				; store exception gate marker
    pop rax				; get the exception gate back
    shr rax, 16
    stosw				; store the high word (31..16) of the address
    shr rax, 16
    stosd				; store the extra high dword (63..32) of the address.
    xor rax, rax
    stosd				; reserved
    dec rcx
    jnz make_exception_gates

    mov rcx, 256-32
make_interrupt_gates: 			; make gates for the other interrupts
    mov rax, interrupt_gate
    push rax			; save the interrupt gate to the stack for later use
    stosw				; store the low word (15..0) of the address
    mov ax, SYS64_CODE_SEL
    stosw				; store the segment selector
    mov ax, 0x8F00
    stosw				; store interrupt gate marker
    pop rax				; get the interrupt gate back
    shr rax, 16
    stosw				; store the high word (31..16) of the address
    shr rax, 16
    stosd				; store the extra high dword (63..32) of the address.
    xor rax, rax
    stosd				; reserved
    dec rcx
    jnz make_interrupt_gates

    ; Set up the exception gates for all of the CPU exceptions
    ; The following code will be seriously busted if the exception gates are moved above 16MB
    mov word [0x00*16], exception_gate_00
    mov word [0x01*16], exception_gate_01
    mov word [0x02*16], exception_gate_02
    mov word [0x03*16], exception_gate_03
    mov word [0x04*16], exception_gate_04
    mov word [0x05*16], exception_gate_05
    mov word [0x06*16], exception_gate_06
    mov word [0x07*16], exception_gate_07
    mov word [0x08*16], exception_gate_08
    mov word [0x09*16], exception_gate_09
    mov word [0x0A*16], exception_gate_10
    mov word [0x0B*16], exception_gate_11
    mov word [0x0C*16], exception_gate_12
    mov word [0x0D*16], exception_gate_13
    mov word [0x0E*16], exception_gate_14
    mov word [0x0F*16], exception_gate_15
    mov word [0x10*16], exception_gate_16
    mov word [0x11*16], exception_gate_17
    mov word [0x12*16], exception_gate_18
    mov word [0x13*16], exception_gate_19

    mov rdi, 0x21			; Set up Keyboard handler
    mov rax, keyboard
    call create_gate
    mov rdi, 0x22			; Set up Cascade handler
    mov rax, cascade
    call create_gate
    mov rdi, 0x28			; Set up RTC handler
    mov rax, rtc
    call create_gate

    lidt [IDTR64]			; load IDT register

    ret


; -----------------------------------------------------------------------------
; Default exception handler
exception_gate:
    mov rsi, int_string
    call os_print_string
    mov rsi, exc_string
    call os_print_string
exception_gate_halt:
    cli				; Disable interrupts
    hlt				; Halt the system
    jmp exception_gate_halt
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; Default interrupt handler
interrupt_gate:				; handler for all other interrupts
    iretq
; -----------------------------------------------------------------------------
