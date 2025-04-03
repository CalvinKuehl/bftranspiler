section .text
global _start
_start:
	inc rsp
	cmp byte [rsp], 0
	je ej1
	bj1:
	cmp byte [rsp], 0
	je ej2
	bj2:
	cmp byte [rsp], 0
	jnz bj2
	ej2:
	cmp byte [rsp], 0
	je ej3
	bj3:
	cmp byte [rsp], 0
	jnz bj3
	ej3:
	cmp byte [rsp], 0
	jnz bj2
	ej2:
	mov rax, 0x3c
	syscall