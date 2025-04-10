all:
	nasm -f elf64 -o asm.o asm.asm
	ld -o asm asm.o
