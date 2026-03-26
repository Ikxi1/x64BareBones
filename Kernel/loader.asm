GLOBAL _loader
global VesaModeInfoBlockBuffer
EXTERN _main
extern init_int
extern init_LUT
extern nv_init

STACKSIZE equ 0x4000        ; that's 16k.

_loader:
      ; important to have this first, apparently
      mov Rsp, stack+STACKSIZE ; set up the stac
      cli

      jrcxz .L1
      mov qword [VesaModeInfoBlockBuffer], rax
      call nv_init    ; graphics mode
.L1:
      call init_int
      call init_LUT   ; populate the keyboard scancode LUT
      sti
      call _main      ; call kernel proper
.L0:
      hlt             ; halt machine should kernel return
      JMP .L0


VesaModeInfoBlockBuffer: dq 1


eokl    dd STACKSIZE + stack
section .bss
      align 32
stack:
      resb STACKSIZE     ; reserve 16k stack on a quadword boundary
