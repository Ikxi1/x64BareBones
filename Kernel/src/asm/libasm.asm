global cpuVendor
global strlen
global gettime
global inportb
global outportb

section .text

cpuVendor:
	; arg 1 = rdi char *buf
	enter 0, 0
	push rbx

	mov rax, 0x0
	cpuid
	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx
	mov byte [rdi+13], 0
	mov rax, rdi

	pop rbx
	mov rsp, rbp
	pop rbp
	ret


strlen:
	; arg 1 = rdi char *string
	; return = rcx string_length
	enter 0, 0
	push rax
	xor rax, rax
	xor rcx, rcx
	cld
	repne scasb
	sub rdi, rcx ; reset string pointer
	sub rcx, 1
	pop rax
	leave
	ret


inportb:
	; arg 1 = di unsigned short port
	mov dx, di
	in al, dx
	ret


outportb:
	; arg 1 = di unsigned short port
	; arg 2 = sil unsigned char data
	mov dx, di
	mov al, sil
	out dx, al
	ret

