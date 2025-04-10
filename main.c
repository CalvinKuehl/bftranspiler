#include <stdio.h>
#include <stdint.h>
// transcompiler for brainfuck to x86_64 assembly (linux system calls)

struct stck {
	int stackptr; // int is used to support negatives (for an empty stack)
	uint8_t items[256]; // each item is an unsigned 8 bit integer, meaning that up to 256 unique values can be entered
};

typedef struct stck stack;

void push(stack *s, uint8_t item) {
	s->stackptr++;
	s->items[s->stackptr] = item;
}

uint8_t pop(stack *s) {
	s->stackptr--;
	return s->items[s->stackptr+1];
}

int main(int argc, char* argv[]) {
	// needed variables
	FILE *outfile;
	FILE *infile;
	char givenchar;
	char lastchar = 'a'; // doesn't matter what this is as long as it is not one of the brainfuck commands
	int bjmpnum = 0;
	int ejmpnum;
	stack ejmpstack;
	ejmpstack.stackptr = -1;
	int iternum = 1;
	
	// reset write file
	outfile = fopen(argv[1], "w");//make sure the file exists so remove() doesnt cause an error
	fclose(outfile);
	remove(argv[1]);
	
	// open write file (assembly x86_64 with linux system calls)
	outfile = fopen(argv[1], "w");
	
	// open read file (brainfuck)
	infile = fopen(argv[2], "r");
	
	// write starter code for the assembly (defining main function, etc)
	fprintf(outfile, "section .text\nglobal _start\n_start:\n\tinc rsp");
	
	// write the body of the code (what is actually compiled from the brainfuck coode)
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
				fprintf(outfile, "\n\tcmp byte [rsp], 0\n\tje ej%i\n\tbj%i:\n\t", bjmpnum, bjmpnum);
				push(&ejmpstack, bjmpnum);
				bjmpnum++;
				break;
			case ']':
				ejmpnum = pop(&ejmpstack);
				fprintf(outfile, "\n\tcmp byte [rsp], 0\n\tjnz bj%i\n\tej%i:\n\t", ejmpnum, ejmpnum);
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
