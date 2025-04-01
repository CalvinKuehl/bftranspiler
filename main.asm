section .text
global _start
_start:
	cmp byte [rsp], 0
	jne ej1 ;jump to end jump 1 if the pointer is zero
	bj1: ; begin jump 1
	ej1: ; end jump 1
	cmp byte [rsp], 0
	jnz bj1 ;jump to begin jump 1 if the pointer is nonzero
	mov rax, 0x3c
	syscall