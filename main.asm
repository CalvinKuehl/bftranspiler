section .data
; boring file reading stuff
ifN	db	"main.bf",0
ifD	dq	0
ifF	dq	0102o
ifM	dq	00600o
; boring file editing stuff
ofN	db	"asm.asm",0
ofD	dq	0
ofF	dq	0102o
ofM	dq	00600o
start	db	"section .text",10,"global _start",10,"_start:",10,9,"inc rsp"
; fun file editing stuff
incbyte	db	10,9,"inc byte [rsp]"
decbyte	db	10,9,"dec byte [rsp]"
incptr	db	10,9,"inc rsp"
decptr	db	10,9,"dec rsp"
bloop	db	10,9,"cmp byte [rsp], 0",10,9,"jnz ej#",10,9,"bj#:"
eloop	db	10,9,"cmp byte [rsp], 0",10,9,"jnz bj#",10,9,"ej#:"

section .text
global _start
_start:
	; make sure the output file exists 
	mov	rdi,	ofN	;const char *filename
	mov	rdx,	[ofM]	;open mode (read + write)
	mov	rsi,	[ofF]	;open flags
	mov	rax,	0x2	;sys_open call number
	syscall
	; remove the output file (to reset it)
	mov	rdi,	ofN	;const char *pathname
	mov	rax,	0x57	;sys_unlink call number
	syscall
	; recreate the output file
	mov	rdi,	ofN	;const char *filename
	mov	rdx,	[ofM]	;open mode (read + write)
	mov	rsi,	[ofF]	;open flags
	mov	rax,	0x2	;sys_open call number
	syscall
	mov	[ofD],	rax	;get the file descriptor
	mov	rsi,	start	;const char *buf (sys_write contents)
	mov	rdx,	43	;size_t count (length of sys_write contents
	mov	rdi,	[ofD]	;file descriptor (so it writes to the file instead of the console)
	mov	rax,	0x1	;sys_write call number
	syscall
	mov	rdi,	[ofD]	;file descriptor (so it closes the file)
	mov	rax,	0x3	;sys_close call number
	syscall
	; exit syscall
	mov	rax,	0x3c	;sys_exit call number
	syscall
	mov rax, 0x3c
	syscall