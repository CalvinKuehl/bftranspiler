section .text
global _start
_start:
	inc rsp
	add byte [rsp], 10
	mov rsi, rsp
	mov rdx, 1
	mov rdi, 0
	mov rax, 0x1
	syscall
	mov rax, 0x3c
	syscall