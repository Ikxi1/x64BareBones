global _loader
global VesaModeInfoBlockBuffer
global process_list
global screen_text

extern _main
extern init_int
extern init_LUT
extern nv_init
extern heap_init
extern init_console
extern initProcess
extern naive_terminal

; define process struct and list
%define MAX_PROCESSES 16
%define PROCESS_SIZE 32    ; struct size in bytes
; struct as offsets
%define PROCESS_PID    0
%define PROCESS_STACK  8
%define PROCESS_RSP    16

STACKSIZE equ 0x4000        ; that's 16k.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

_loader:
      ; important to have this first, apparently
      mov Rsp, stack+STACKSIZE ; set up the stac

      cli

      jrcxz .L1
      mov qword [VesaModeInfoBlockBuffer], rax
      call nv_init    ; graphics mode
.L1:
      call heap_init
      call init_int
      call init_LUT   ; populate the keyboard scancode LUT
      call init_console

      lea rax, [process_list]
      mov qword [rax + PROCESS_PID], 0
      mov qword [rax + PROCESS_STACK], stack
      mov qword [rax + PROCESS_RSP], 0

      ;lea rdi, [naive_terminal]
      ;lea rsi, [process_list]
      ;call initProcess

      sti
      call naive_terminal

.L0:
      hlt             ; halt machine should kernel return
                      ; and inbetween scheduling
      JMP .L0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

VesaModeInfoBlockBuffer: dq 1  ; hold the pointer to the block
                               ; after the bootloader ran

eokl    dq  STACKSIZE + stack

section .data

align 8
screen_text:
      dd 25 * 80
      dd 0
      dq screen_text_buffer

section .bss

align 8
process_list:
      resb PROCESS_SIZE * MAX_PROCESSES

screen_text_buffer:
      resb 25 * 80

align 32 ; align so high for SIMD
stack:
      resb STACKSIZE     ; reserve 16k stack on a quadword boundary
