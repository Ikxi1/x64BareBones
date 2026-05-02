%include "macros.inc"

global init_int

extern keyb_irq
extern pit_irq
extern timer_init
extern schedule
extern process_list

; Set up interrupt handlers
init_int:
      ; the PIT IRQ
      ; this overwrites Pure64's RTC interrupt
      mov rdi, 0x20
      mov rax, pit_handler
      call create_gate

      ; map kb IRQ
      ; this overwrites Pure64's kb interrupt
      mov rdi, 0x21
      mov rax, keyboard
      call create_gate

      lidt [IDTR64]

      call init_pic

      call timer_init

ret


init_pic:
      ; Enable specific interrupts
      in al, 0x21
      mov al, 11111100b       ; Enable Keyboard 1 and PIT 0
      out 0x21, al

ret


; -----------------------------------------------------------------------------
; Keyboard interrupt. IRQ 0x01, INT 0x21
; This IRQ runs whenever there is input on the keyboard
align 16
keyboard:
      pushaq

      call keyb_irq

      ; testing process switching
      ;mov rdi, rsp
      ;call schedule
      ;mov rsp, rax

keyboard_done:
      mov al, 0x20            ; Acknowledge the IRQ
      out 0x20, al

      popaq
iretq
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; PIT interrupt. IRQ 0x00, INT 0x20
align 16
pit_handler:
      pushaq

      call pit_irq

      mov rdi, rsp
      lea rsi, [process_list]
      call schedule
      mov rsp, rax

pit_done:
      mov al, 0x20           ; Acknowledge the IRQ
      out 0x20, al

      popaq
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
