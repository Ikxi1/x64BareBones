GLOBAL int80
GLOBAL int81
GLOBAL int82
GLOBAL switchToCurrent
EXTERN syscallsImpl
EXTERN switchContext
EXTERN switchToCurrentPID
EXTERN nestedSti
EXTERN nestedCli
EXTERN getKernelContext

int80:
	cli

	push rDI
	push rSI
	push rDX
	push rCX
	push rBX
	push rAX

	call syscallsImpl

	pop rBX
	pop rBX
	pop rCX
	pop rDX
	pop rSI
	pop rDI

;	sti
	iretq

int81:
	cli
	push rax

	push rSP				; Save the old proccess stack
	call getKernelContext	; Get the kernel stack
	mov rSP, rAX			; Switch to the kernel stack
	call switchContext		; Get the new process stack
	mov rSP, rAX			; Switch to the new process stack

	pop rax
;	sti			; We don't need to sti! the interruption flag is saved on stack!!
				; Besides this way we can use nested system calls, as the
				; corresponding flags are properly stacked
	iretq

