all:
	nasm -f elf64 -o asm.o main.asm
	ld -o asm asm.o