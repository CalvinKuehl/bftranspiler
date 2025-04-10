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
end	db	10,9,"mov rax, 0x3c",10,9,"syscall"
; fun file editing stuff
incbyte	db	10,9,"inc byte [rsp]"
decbyte	db	10,9,"dec byte [rsp]"
incptr	db	10,9,"inc rsp"
decptr	db	10,9,"dec rsp"
bloop	db	10,9,"cmp byte [rsp], 0",10,9,"jnz ej#",10,9,"bj#:"
eloop	db	10,9,"cmp byte [rsp], 0",10,9,"jnz bj#",10,9,"ej#:"
outbyte	db	10,9,"mov rsi, rsp",10,9,"mov rdx, 1",10,9,"mov rax, 1",10,9,"syscall"
inbyte	db	10,9,"mov rsi, rsp",10,9,"mov rdx, 1",10,9,"mov rax, 0",10,0,"syscall"
gchar	db	0	;given character

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
	; write the starter code to the output file
	mov	[ofD],	rax	;get the file descriptor
	mov	rsi,	start	;const char *buf (sys_write contents)
	mov	rdx,	44	;size_t count (length of sys_write contents)
	mov	rdi,	[ofD]	;unsigned int fd (file descriptor so it writes to the file instead of the console)
	mov	rax,	0x1	;sys_write call number
	syscall
	; get input file
	mov	rdi,	ifN	;const char *filename
	mov	rdx,	[ofM]	;open mode (read + write)
	mov	rsi,	[ifF]	;open flags
	mov	rax,	0x2	;sys_open call number
	syscall
	mov	[ifD],	rax	;get file descriptor
	; prepare everything else that is needed
	mov	r8,	1
	; begin writing the output file
	ltc:			;loop through characters
	; read byte from input file
	mov	rsi,	gchar	;char *buf (inputted message)
	mov	rdx,	1	;size_t count (length of inputted message, 1 byte)
	mov	rdi,	[ifD]	;unsigned int fd (file descriptor)
	mov	rax,	0x0	;sys_read call number
	syscall
	mov	r8,	[gchar]
	cmp	r8,	0x3e	;if (gchar == '>')
	jne cfi0
	mov	rsi,	incptr
	mov	rdx,	9
	cfi0:
	cmp	r8,	0x3c	;if (gchar == '<')
	jne cfi1
	mov	rsi,	decptr
	mov	rdx,	9
	cfi1:
	eltc:			;end of loop through characters
	; write the output to the file
	mov	rdi,	[ofD]	;unsigned int fd (file descriptor)
	mov	rax,	0x1
	syscall
	cmp	r8,	0x3b	;jump to beginning of loop (semicolon will be indicator for end of file)
	jne	ltc
	; put the ending code at the end of the file (exit syscall)
	mov	rsi,	end
	mov	rdx,	24
	mov	rdi,	[ofD]
	mov	rax,	0x1
	syscall
	; close output file
	mov	rdi,	[ofD]	;unsigned int fd (file descriptor so it closes the file)
	mov	rax,	0x3	;sys_close call number
	syscall
	; close input file
	mov	rdi,	[ifD]	;unsigned int fd (file descriptor so it closes the file)
	mov	rax,	0x3	;sys_close call number
	syscall
	; exit syscall
	mov	rax,	0x3c	;sys_exit call number
	syscall