GLOBAL loadLDT
GLOBAL saveLDT

saveLDT:
	mov EAX, [ESP+4]	;Store in EAX the 6 byte array address
	sldt [EAX]			;Store in the GDT the array
	ret

loadLDT:
	lldt    [ESP+4]			; Done loading the GDT
	ret
