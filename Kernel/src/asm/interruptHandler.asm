GLOBAL int_08_hand
GLOBAL int_09_hand
EXTERN switchContext
EXTERN keyboardHandler
EXTERN getKernelContext

; Handler of timer tick interrupt
int_08_hand:
    cli
    push rax
                            ; The Switching Context
    push rSP				; Save the old proccess stack
    call getKernelContext	; Get the kernel stack
    mov rSP, rAX			; Switch to the kernel stack
    call switchContext		; Get the new process stack
    mov rSP, rAX			; Switch to the new process stack

    mov al, 20h		; Envio de EOI generico al PIC
    out 20h, al

    pop rax
;	sti			; We don't need to sti! the interruption flag is saved on stack!!
                ; Besides this way we can use nested system calls, as the
                ; corresponding flags are properly stacked
    iretq

; Handler of keyboard interrupt
int_09_hand:
    cli
    push rax

    in al, 60h
    push rax
    call keyboardHandler
    pop rax

    mov al, 20h		; Envio de EOI generico al PIC
    out 20h, al

    pop rax
;	sti			; We don't need to sti! the interruption flag is saved on stack!!
                ; Besides this way we can use nested system calls, as the
                ; corresponding flags are properly stacked

    iretq
