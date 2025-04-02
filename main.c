#include <stdio.h>
// transcompiler for brainfuck to x86_64 assembly (linux system calls)

int main(int argc, char* argv[]) {
	FILE *outfile;
	FILE *infile;
	char givenchar;
	int jmpnum = 0;
	// reset write file
	outfile = fopen(argv[1], "w");
	fclose(outfile);
	remove(argv[1]);
	// open write file (asm)
	outfile = fopen(argv[1], "w");
	// open read file (brainfuck)
	infile = fopen(argv[2], "r");
	// get starter code for the assembly (defining main function, etc)
	fprintf(outfile, "section .text\nglobal _start\n_start:\n\tinc rsp");
	// get the rest of the code (what is actually from the brainfuck coode)
	givenchar = fgetc(infile);
	while ( givenchar != EOF) {
		switch (givenchar) {
			case '+': // increment byte
				fprintf(outfile, "\n\tinc byte [rsp]");
				break;
			case '-': // decrement byte
				fprintf(outfile, "\n\tdec byte [rsp]");
				break;
			case '.': // output byte
				fprintf(outfile, "\n\tmov rsi, rsp\n\tmov rdx, 1\n\tmov rdi, 0\n\tmov rax, 0x1\n\tsyscall");
				break;
			case ',': // input byte
				fprintf(outfile, "\n\tmov rsi, rsp\n\tmov rdx, 1\n\tmov rdi, 0\n\tmov rax, 0x0\n\tsyscall");
				break;
			case '>': // increment pointer
				fprintf(outfile, "\n\tinc rsp");
				break;
			case '<': // decrement pointer
				fprintf(outfile, "\n\tdec rsp");
				break;
			case '[': // begin loop
				jmpnum++;
				fprintf(outfile, "\n\tcmp byte [rsp], 0\n\tje ej%i\n\tjmp bj%i\n\tbj%i:", jmpnum, jmpnum, jmpnum);
				break;
			case ']':
				fprintf(outfile, "\n\tcmp byte [rsp], 0\n\tjnz bj%i\n\tej%i:", jmpnum, jmpnum);
				break;
			default:
				break;
		}
		givenchar = fgetc(infile);
	}
	// get the standard system exit call
	fprintf(outfile, "\n\tmov rax, 0x3c\n\tsyscall");
	fclose(outfile);
	return 0;
}