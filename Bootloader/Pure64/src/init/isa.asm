; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; INIT ISA
; =============================================================================


init_isa:
      mov edi, 0x00004000		; Clear out memory for the E820 map
      xor eax, eax
      mov ecx, 2048
      rep stosd

; Get the BIOS E820 Memory Map
; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi
do_e820:
      mov edi, 0x00004000		; location that memory map will be stored to
      xor ebx, ebx			; ebx must be 0 to start
      xor bp, bp			; keep an entry count in bp
      mov edx, 0x0534D4150		; Place "SMAP" into edx
      mov eax, 0xe820
      mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
      mov ecx, 24			; ask for 24 bytes
      int 0x15
      jc nomemmap			; carry set on first call means "unsupported function"
      mov edx, 0x0534D4150		; Some BIOSes apparently trash this register?
      cmp eax, edx			; on success, eax must have been reset to "SMAP"
      jne nomemmap
      test ebx, ebx			; ebx = 0 implies list is only 1 entry long (worthless)
      je nomemmap
      jmp jmpin
e820lp:
      mov eax, 0xe820			; eax, ecx get trashed on every int 0x15 call
      mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
      mov ecx, 24			; ask for 24 bytes again
      int 0x15
      jc memmapend			; carry set means "end of list already reached"
      mov edx, 0x0534D4150		; repair potentially trashed register
jmpin:
      jcxz skipent			; skip any 0 length entries
      cmp cl, 20			; got a 24 byte ACPI 3.X response?
      jbe notext
      test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
      je skipent
notext:
      mov ecx, [es:di + 8]		; get lower dword of memory region length
      test ecx, ecx			; is the qword == 0?
      jne goodent
      mov ecx, [es:di + 12]		; get upper dword of memory region length
      jecxz skipent			; if length qword is 0, skip entry
goodent:
      inc bp				; got a good entry: ++count, move to next storage spot
      add di, 32
skipent:
      test ebx, ebx			; if ebx resets to 0, list is complete
      jne e820lp
nomemmap:
      mov byte [cfg_e820], 0		; No memory map function
memmapend:
      xor eax, eax			; Create a blank record for termination (32 bytes)
      mov ecx, 8
      rep stosd

; Enable the A20 gate
set_A20:
      in al, 0x64
      test al, 0x02
      jnz set_A20
      mov al, 0xD1
      out 0x64, al
check_A20:
      in al, 0x64
      test al, 0x02
      jnz check_A20
      mov al, 0xDF
      out 0x60, al


      ;;; GRAPHICS SETUP ;;;

      ; Configure graphics if requested
      cmp byte [cfg_vesa], 1        ; Check if VESA should be enabled
      jne VBEdone                         ; If not then skip VESA init

      ; get VesaInfoBlock
      mov di, VesaInfoBlockBuffer
      mov ax, 0x4F00
      int 0x10
      cmp ax, 0x004F
      jne VBEfail

      ; get video modes
      mov bx, [VesaInfoBlockBuffer + VesaInfoBlock.VideoModesOffset]
      mov es, [VesaInfoBlockBuffer + VesaInfoBlock.VideoModesSegment]
      mov cx, [es:bx]
      cmp cx, 0xFFFF
      je VBEnomodes

      ; counter for modes
      mov si, ScratchBuffer+8
      mov byte [si], 0
      mov dword [si+24], "vi"
      mov dword [si+40], "de"

mode_loop:
      mov ax, 0x4F01
      mov di, VesaModeInfoBlockBuffer
      int 0x10
      cmp ax, 0x004F
      jne VBEfail

      ; print mode number
      mov di, ScratchBuffer
      add di, 8
      movzx ax, byte [di]
      sub di, 8 ; where we want to number
      call os_int_to_string_16
      mov si, di
      call print_string_16
      mov si, ScratchBuffer
      mov byte [si], ')'
      mov byte [si+1], ' '
      mov byte [si+2], 0
      call print_string_16

      ; print data
      mov ax, [VesaModeInfoBlockBuffer + VesaModeInfoBlock.Width]
      mov di, ScratchBuffer
      call os_int_to_string_16
      mov si, di
      call print_string_16
      mov si, ScratchBuffer
      mov byte [si], 'x'
      mov byte [si+1], 0
      call print_string_16

      mov ax, [VesaModeInfoBlockBuffer + VesaModeInfoBlock.Height]
      mov di, ScratchBuffer
      call os_int_to_string_16
      mov si, di
      call print_string_16
      mov si, ScratchBuffer
      mov byte [si], 'x'
      mov byte [si+1], 0
      call print_string_16

      movzx ax, byte [VesaModeInfoBlockBuffer + VesaModeInfoBlock.BitsPerPixel]
      mov di, ScratchBuffer
      call os_int_to_string_16
      mov si, di
      call print_string_16
      mov si, ScratchBuffer
      mov byte [si], ' '
      mov byte [si+1], 0
      call print_string_16

      mov di, [ScratchBuffer+8]
      inc di
      mov [ScratchBuffer+8], di

      ; loop over the different modes later
VBEnextmode:
      add bx, 2
      mov cx, [es:bx]
      cmp cx, 0xFFFF
      jne mode_loop

      pusha
      mov ax, 0x0200
      mov bx, 0    ; page
      mov dh, 24   ; row
      mov dl, 0    ; column
      int 0x10
      popa

      mov si, ScratchBuffer
      mov byte [si], '0'
      mov byte [si+1], 0
      call print_string_16

; need to unmask IRQ1 and enable interrupts (sti)
; don't forget to mask later again and cli
VBEselectmode:
      sti                 ; interrupts must be enabled
      mov al, 0xFD        ; unmask IRQ1 (keyboard)
      out 0x21, al
      mov al, 0xFF        ; keep slave masked
      out 0xA1, al

      xor dx, dx          ; will hold the selector
      call os_print_newline_16 ; why no worky?

; 72 arrow up
; 80 arrow down
; 28 enter
VBEselectmodeloop:
      movzx si, byte [ScratchBuffer+8]

      mov ah, 0
      int 0x16

      cmp ah, 72 ; arrow up
      jne L0
      cmp dx, si ; check overflow
      jge L1
      inc dx
      jmp L3
L1:
      xor dl, dl
L0:
      cmp ah, 80 ; arrow down
      jne L2
      cmp dl, 0  ; check underflow
      jle L2
      dec dl
      jmp L3
L2:
      cmp ah, 28 ; enter
      jne VBEselectmodeloop
      ; save selected mode
      mov si, ScratchBuffer+16
      mov [si], dl

      ; get VesaInfoBlock
      mov di, VesaInfoBlockBuffer
      mov ax, 0x4F00
      int 0x10
      cmp ax, 0x004F
      jne VBEfail

      ; get video modes
      mov bx, [VesaInfoBlockBuffer + VesaInfoBlock.VideoModesOffset]
      mov es, [VesaInfoBlockBuffer + VesaInfoBlock.VideoModesSegment]
      mov cx, [es:bx]
      cmp cx, 0xFFFF
      je VBEnomodes
      ; loop over modes again, but this time set mode in dl
      xor dh, dh
mode_loop2:
      mov ax, 0x4F01
      mov di, VesaModeInfoBlockBuffer
      int 0x10
      cmp ax, 0x004F
      jne VBEfail

      cmp dh, dl
      je VBEselectmode2

      add bx, 2
      mov cx, [es:bx]
      cmp cx, 0xFFFF
      inc dh
      jmp mode_loop2

L3:                ; print selected mode number
      ; move cursor to bottom right
      pusha
      mov ax, 0x0200
      mov bx, 0    ; page
      mov dh, 24   ; row
      mov dl, 0    ; column
      int 0x10
      popa

      movzx ax, dl
      mov di, ScratchBuffer
      call os_int_to_string_16
      mov si, di
      call print_string_16

      mov si, ScratchBuffer
      mov byte [si], ' '
      mov byte [si+1], 0
      call print_string_16

      jmp VBEselectmodeloop

VBEselectmode2:
      cli
      mov bx, cx
      mov ax, 0x4F02
      int 0x10
      cmp ax, 0x004F
      jne VBEfail
      jmp VBEdone

VBEnomodes:
      mov si, msg_novesamodes
      call print_string_16
      mov byte [cfg_vesa], 0
      jmp VBEdone

VBEfail:
      mov si, msg_novesa
      call print_string_16
      mov byte [cfg_vesa], 0		; Clear the VESA config as it was not successful
halt:
      hlt
      jmp halt


VBEdone:

; Set up RTC
; Port 0x70 is RTC Address, and 0x71 is RTC Data
; http://www.nondot.org/sabre/os/files/MiscHW/RealtimeClockFAQ.txt
rtc_poll:
      mov al, 0x0A			; Status Register A
      out 0x70, al			; Select the address
      in al, 0x71		    ; Read the data
      test al, 0x80			; Is there an update in process?
      jne rtc_poll			; If so then keep polling
      mov al, 0x0A			; Status Register A
      out 0x70, al			; Select the address
      mov al, 00100110b		; UIP (0), RTC@32.768KHz (010), Rate@1024Hz (0110)
      out 0x71, al			; Write the data

      ; Remap PIC IRQ's
      mov al, 00010001b		; begin PIC 1 initialization
      out 0x20, al
      mov al, 00010001b		; begin PIC 2 initialization
      out 0xA0, al
      mov al, 0x20			; IRQ 0-7: interrupts 20h-27h
      out 0x21, al
      mov al, 0x28			; IRQ 8-15: interrupts 28h-2Fh
      out 0xA1, al
      mov al, 4
      out 0x21, al
      mov al, 2
      out 0xA1, al
      mov al, 1
      out 0x21, al
      out 0xA1, al

      ; Mask all PIC interrupts
      mov al, 0xFF
      out 0x21, al
      out 0xA1, al

ret


; =============================================================================
; EOF
