; =============================================================================
; Pure64 -- a 64-bit OS loader written in Assembly for x86-64 systems
; Copyright (C) 2008-2014 Return Infinity -- see LICENSE.TXT
;
; System Calls
; =================================================================

; when including in another file
; add USE16 before the include, if USE32 or 64 have been called previously

; -----------------------------------------------------------------------------
; os_move_cursor_16 -- Moves the virtual cursor in text mode
;  IN:	AH, AL = row, column
; OUT:	Nothing. All registers preserved
os_move_cursor_16:
    push cx
    push bx
    push ax

    xor bx, bx
    mov [screen_cursor_x], ah
    mov [screen_cursor_y], al
    mov bl, ah

    ; Calculate the new offset
    and ax, 0x00FF      ; only keep the low 8 bits
    mov cl, 80
    mul cl              ; AX = AL * CL
    add ax, bx
    shl ax, 1           ; multiply by 2

; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; doesn't work at the moment because B8000 is 20 bit
    ; add ax, 0xB8000
    ; mov [screen_cursor_offset], ax

    pop ax
    pop bx
    pop cx
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; os_print_newline_16 -- Reset cursor to start of next line and scroll if needed
;  IN:	Nothing
; OUT:	Nothing, all registers perserved
os_print_newline_16:
    push ax

    mov ah, 0                      ; Set the cursor x value to 0
    mov al, [screen_cursor_y]      ; Grab the cursor y value
    cmp al, 24                     ; Compare to see if we are on the last line
    je os_print_newline_scroll_16  ; If so then we need to scroll the sreen

    inc al                         ; If not then we can go ahead an increment the y value
    jmp os_print_newline_done_16

os_print_newline_scroll_16:
    mov ax, 0x0000                 ; If we have reached the end then wrap back to the front

os_print_newline_done_16:
    call os_move_cursor_16         ; update the cursor

    pop ax
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; os_print_string_16 -- Displays text
;  IN:	RSI = message location (zero-terminated string)
; OUT:	Nothing, all registers perserved
os_print_string_16:
    push si
    push ax

    cld				; Clear the direction flag.. we want to increment through the string

os_print_string_nextchar_16:
    lodsb				; Get char from string and store in AL
    cmp al, 0			; Strings are Zero terminated.
    je os_print_string_done_16		; If char is Zero then it is the end of the string

    cmp al, 13			; Check if there was a newline character in the string
    je os_print_string_newline_16	; If so then we print a new line

    call os_print_char_16

    jmp os_print_string_nextchar_16

os_print_string_newline_16:
    call os_print_newline_16
    jmp os_print_string_nextchar_16

os_print_string_done_16:
    pop ax
    pop si
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; os_print_char_16 -- Displays a char in 16bit mode
;  IN:	AL = char to display
; OUT:	Nothing. All registers preserved
; increment cursor offset and x, if (x >= 79) x = 0; y++;
os_print_char_16:
    push ax
    push di

    ; write char to mem
    mov di, [screen_cursor_offset]
    stosb
    add word [screen_cursor_offset], 2	; Add 2 (1 byte for char and 1 byte for attribute)

    ; if (x >= 79 && y >= 24)
    ; cmp [screen_cursor_x]
    ; lea ax, [screen_cursor_base]
    ; lea di, [screen_cursor_offset]

    pop di
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; os_print_char_hex_16 -- Displays a char in hex mode in 16bit mode
;  IN:	AL = char to display
; OUT:	Nothing. All registers preserved
os_print_char_hex_16:
    push bx
    push ax

    mov bx, hextable

    push ax             ; save rax for the next part
    shr al, 4           ; we want to work on the high part so shift right by 4 bits
    xlatb
    call os_print_char

    pop ax
    and al, 0x0f        ; we want to work on the low part so clear the high part
    xlatb
    call os_print_char

    pop ax
    pop bx
    ret
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; os_int_to_string_16 -- Convert a binary integer into an string string in 16bit mode
; IN:   RAX = binary integer
;       RDI = pointer to string
; OUT:  RDI = pointer to string zero-terminated
;       All other registers preserved
; Adapted from http://www.cs.usfca.edu/~cruse/cs210s09/rax2uint.s
os_int_to_string_16:
    push si
    push dx
    push cx
    push bx
    push ax

    mov bx, 10                    ; base of the decimal system
    xor cx, cx                    ; number of digits generated
os_int_to_string_next_divide_16:
    xor dx, dx                    ; RAX extended to (RDX,RAX)
    div bx                        ; divide by the number-base
    push dx                       ; save remainder on the stack
    inc cx                        ; and count this remainder
    cmp ax, 0x0                   ; was the quotient zero?
    jne os_int_to_string_next_divide_16  ; no, do another division
    mov si, cx
os_int_to_string_next_digit_16:
    pop dx                        ; else pop recent remainder
    add dl, '0'                   ; and convert to a numeral
    mov [di], dl                  ; store to memory-buffer
    inc di
    loop os_int_to_string_next_digit_16  ; again for other remainders
    mov al, 0x00
    stosb                         ; Store the null terminator at the end of the string
    inc si
    sub di, si

    pop ax
    pop bx
    pop cx
    pop dx
    pop si
    ret
; -----------------------------------------------------------------------------


; =============================================================================
; EOF
