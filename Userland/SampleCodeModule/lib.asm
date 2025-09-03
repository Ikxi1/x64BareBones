; Write section of memory to all zero
; arg 1 = rdi void *destiny
; arg 2 = rsi uint64_t length

; global memz

; memz:
; 	xor r8, r8 ; thing to put into *destiny
; 	mov rcx, rdi
; 	add rcx, rsi ; end of destiny
; .l1:
; 	mov rax, rsi ; figure out if divisible by 8
; 	mov rdx, 8
; 	div rdx
; 	cmp rdx, 8
; 	jne .l2

; 	mov [rdi], qword rax
; 	add rdi, 8
; 	cmp rdi, rcx
; 	jne .l1
; .l2:

; 	cmp rdx, 4
; .l3:
; 	cmp sil, 2
; .l4:

; .ret:
; 	ret
