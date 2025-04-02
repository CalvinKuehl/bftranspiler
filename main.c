#include <stdio.h>
// transcompiler for brainfuck to x86_64 assembly (linux system calls)

int main(int argc, char* argv[]) {
	// needed variables
	FILE *outfile;
	FILE *infile;
	char givenchar;
	char lastchar = 'd';
	int jmpnum = 0;
	int iternum = 1;
	
	// reset write file
	outfile = fopen(argv[1], "w");
	fclose(outfile);
	remove(argv[1]);
	
	// open write file (assembly x86_64 with linux system calls)
	outfile = fopen(argv[1], "w");
	
	// open read file (brainfuck)
	infile = fopen(argv[2], "r");
	
	// get starter code for the assembly (defining main function, etc)
	fprintf(outfile, "section .text\nglobal _start\n_start:\n\tinc rsp");
	
	// get the rest of the code (what is actually from the brainfuck coode)
	givenchar = fgetc(infile);
	do {
		lastchar = givenchar;
		givenchar = fgetc(infile);
		switch (lastchar) {
			case '+': // increment byte
				if (lastchar == givenchar) {
					iternum++;
				} else {
					if (iternum == 1) {
						fprintf(outfile, "\n\tinc byte [rsp]");
					} else {
						fprintf(outfile, "\n\tadd byte [rsp], %i", iternum);
						iternum = 1;
					}
				}
				break;
			case '-': // decrement byte
				if (lastchar == givenchar) {
					iternum++;
				} else {
					if (iternum == 1) {
						fprintf(outfile, "\n\tdec byte [rsp]");
					} else {
						fprintf(outfile, "\n\tsub byte [rsp], %i", iternum);
						iternum = 1;
					}
				}
				break;
			case '.': // output byte
				fprintf(outfile, "\n\tmov rsi, rsp\n\tmov rdx, 1\n\tmov rdi, 0\n\tmov rax, 0x1\n\tsyscall");
				break;
			case ',': // input byte
				fprintf(outfile, "\n\tmov rsi, rsp\n\tmov rdx, 1\n\tmov rdi, 0\n\tmov rax, 0x0\n\tsyscall");
				break;
			case '>': // increment pointer
				if (lastchar == givenchar) {
					iternum++;
				} else {
					if (iternum == 1) {
						fprintf(outfile, "\n\tinc rsp");
					} else {
						fprintf(outfile, "\n\tadd rsp, %i", iternum);
						iternum = 1;
					}
				}
				break;
			case '<': // decrement pointer
				if (lastchar == givenchar) {
					iternum++;
				} else {
					if (iternum == 1) {
						fprintf(outfile, "\n\tdec rsp");
					} else {
						fprintf(outfile, "\n\tsub rsp, %i", iternum);
						iternum = 1;
					}
				}
				break;
			case '[': // begin loop
				jmpnum++;
				fprintf(outfile, "\n\tcmp byte [rsp], 0\n\tje ej%i\n\tbj%i:", jmpnum, jmpnum);
				break;
			case ']':
				fprintf(outfile, "\n\tcmp byte [rsp], 0\n\tjnz bj%i\n\tej%i:", jmpnum, jmpnum);
				break;
			default:
				break;
		}
	} while ( givenchar != EOF);
	
	// get the standard system exit call
	fprintf(outfile, "\n\tmov rax, 0x3c\n\tsyscall");
	fclose(outfile);
	return 0;
}