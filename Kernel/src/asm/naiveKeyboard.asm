global keyboard
global init_keyb

extern keyb_irq

; Set up Keyboard handler
init_keyb:
    ; map kb IRQ
    mov rdi, 0x21
    mov rax, keyboard
    call create_gate

    lidt [IDTR64]

    call init_pic

    ret


init_pic:
    ; Enable specific interrupts
    in al, 0x21
    mov al, 11111101b		; Enable Keyboard
    out 0x21, al

    ret


; -----------------------------------------------------------------------------
; Keyboard interrupt. IRQ 0x01, INT 0x21
; This IRQ runs whenever there is input on the keyboard
align 16
keyboard:
    push rdi
    push rax

    call keyb_irq

    ; xor rax, rax

    ; in al, 0x60			; Get the scancode from the keyboard
    ; test al, 0x80
    ; jnz keyboard_done

    ; ; call actual keyboard handling later
    ; mov [0x000B8000], al		; Dump the scancode to the screen

keyboard_done:
    mov al, 0x20			; Acknowledge the IRQ
    out 0x20, al

    pop rax
    pop rdi
    iretq
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; create_gate
; rax = address of handler
; rdi = gate # to configure
create_gate:
    push rdi
    push rax

    shl rdi, 4			; quickly multiply rdi by 16
    stosw				; store the low word (15..0)
    shr rax, 16
    add rdi, 4			; skip the gate marker
    stosw				; store the high word (31..16)
    shr rax, 16
    stosd				; store the high dword (63..32)

    pop rax
    pop rdi
ret
; -----------------------------------------------------------------------------


IDTR64:
    dw 256*16-1
    dq 0x00000000