global loader
extern main
extern initializeKernelBinary

loader:


	call initializeKernelBinary	; Set up the kernel binary, and get thet stack address
	mov rsp, rax				; Set up the stack with the returned address
	call main
hang:
	cli
	hlt	; halt machine should kernel return
	jmp hang



; -----------------------------------------------------------------------------
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
; -----------------------------------------------------------------------------


; -----------------------------------------------------------------------------
; os_print_char -- Displays a char
;  IN:	AL = char to display
; OUT:	Nothing. All registers preserved
os_print_char:
	push rdi

	mov rdi, [screen_cursor_offset]
	stosb
	add qword [screen_cursor_offset], 2	; Add 2 (1 byte for char and 1 byte for attribute)

	pop rdi
	ret
; -----------------------------------------------------------------------------
