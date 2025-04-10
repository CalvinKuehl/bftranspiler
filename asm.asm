section .text
global _start
_start:
	inc rsp
	inc rsp
	dec rsp;
	mov rax, 0x3c
	syscall