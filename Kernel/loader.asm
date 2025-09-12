global loader
extern main
extern initializeKernelBinary
extern keyb_irq
extern init_keyb

loader:
    call init_keyb

    call initializeKernelBinary	; Set up the kernel binary, and get thet stack address
    mov rsp, rax				; Set up the stack with the returned address
    call main
hang:
    cli
    hlt	; halt machine in case kernel returns
    jmp hang


; os_print_string -- Displays text
;  IN:	RSI = message location (zero-terminated string)
; OUT:	Nothing, all registers perserved
os_print_string:
    push rsi
    push rax

    cld				; Clear the direction flag.. we want to increment through the string

os_print_string_nextchar:
    lodsb				; Get char from string and store in AL
    cmp al, 0			; Strings are Zero terminated.
    je os_print_string_done		; If char is Zero then it is the end of the string

    cmp al, 13			; Check if there was a newline character in the string
    je os_print_string_newline	; If so then we print a new line

    call os_print_char

    jmp os_print_string_nextchar

os_print_string_newline:
    call os_print_newline
    jmp os_print_string_nextchar

os_print_string_done:
    pop rax
    pop rsi
    ret


; os_print_char -- Displays a char
;  IN:	AL = char to display
; OUT:	Nothing. All registers preserved
os_print_char:
    push rdi

    mov rdi, [0x0000000000005A00 + 8]
    stosb
    add qword [0x0000000000005A00 + 8], 2	; Add 2 (1 byte for char and 1 byte for attribute)

    pop rdi
    ret


; os_print_newline -- Reset cursor to start of next line and scroll if needed
;  IN:	Nothing
; OUT:	Nothing, all registers perserved
os_print_newline:
    push rax

    mov ah, 0			; Set the cursor x value to 0
    mov al, [0x0000000000005A00 + 387]	; Grab the cursor y value
    cmp al, 24			; Compare to see if we are on the last line
    je os_print_newline_scroll	; If so then we need to scroll the sreen

    inc al				; If not then we can go ahead an increment the y value
    jmp os_print_newline_done

os_print_newline_scroll:
    mov ax, 0x0000			; If we have reached the end then wrap back to the front

os_print_newline_done:
    call os_move_cursor		; update the cursor

    pop rax
    ret


; os_move_cursor -- Moves the virtual cursor in text mode
;  IN:	AH, AL = row, column
; OUT:	Nothing. All registers preserved
os_move_cursor:
    push rcx
    push rbx
    push rax

    xor ebx, ebx
    mov [0x0000000000005A00 + 386], ah
    mov [0x0000000000005A00 + 387], al
    mov bl, ah

    ; Calculate the new offset
    and rax, 0x00000000000000FF	; only keep the low 8 bits
    mov cl, 80
    mul cl				; AX = AL * CL
    add ax, bx
    shl ax, 1			; multiply by 2

    add rax, 0x00000000000B8000
    mov [0x0000000000005A00 + 8], rax

    pop rax
    pop rbx
    pop rcx
    ret


; os_int_to_string -- Convert a binary interger into an string string
;  IN:	RAX = binary integer
;	RDI = location to store string
; OUT:	RDI = pointer to end of string
;	All other registers preserved
; Min return value is 0 and max return value is 18446744073709551615 so your
; string needs to be able to store at least 21 characters (20 for the number
; and 1 for the string terminator).
; Adapted from http://www.cs.usfca.edu/~cruse/cs210s09/rax2uint.s
os_int_to_string:
    push rdx
    push rcx
    push rbx
    push rax
    push r8

    mov rbx, 10				; base of the decimal system
    xor rcx, rcx				; number of digits generated
os_int_to_string_next_divide:
    xor rdx, rdx				; RAX extended to (RDX,RAX)
    div rbx					; divide by the number-base
    push rdx				; save remainder on the stack
    inc rcx					; and count this remainder
    cmp rax, 0x0				; was the quotient zero?
    jne os_int_to_string_next_divide	; no, do another division
    mov r8, rcx
os_int_to_string_next_digit:
    pop rdx					; else pop recent remainder
    add dl, '0'				; and convert to a numeral
    mov [rdi], dl				; store to memory-buffer
    inc rdi
    loop os_int_to_string_next_digit	; again for other remainders
    mov al, 0x00
    stosb					; Store the null terminator at the end of the string
    inc r8
    sub rdi, r8

    pop r8
    pop rax
    pop rbx
    pop rcx
    pop rdx
    ret
